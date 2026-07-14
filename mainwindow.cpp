#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QAction>
#include <QStyle>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("个人通讯录 V1.3");
    resize(1500, 800);
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupToolBar();
    setupSearchBar();
    setupTableView();
    setupDetailPanel();
    setupGroupPanel();

    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(5, 5, 5, 5);
    QLabel *searchLabel = new QLabel("搜索:", searchWidget);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchWidget->setMaximumHeight(40);

    leftLayout->addWidget(searchWidget);
    leftLayout->addWidget(tableView);

    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(groupList->parentWidget());
    splitter->addWidget(leftWidget);
    splitter->addWidget(detailPanel);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 0);

    setCentralWidget(splitter);

    connect(searchEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
}

void MainWindow::setupSearchBar()
{
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("输入姓名或电话...");
    searchEdit->setMaximumHeight(30);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("核心操作");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *addAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "新建");
    QAction *deleteAction = toolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), "删除");
    QAction *editAction = toolBar->addAction(style()->standardIcon(QStyle::SP_ArrowBack), "编辑");

    toolBar->addSeparator();

    QAction *addToGroupAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), "加入分组");
    QAction *removeFromGroupAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), "移出分组");

    connect(addAction, &QAction::triggered, this, &MainWindow::onAddContact);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteContact);
    connect(editAction, &QAction::triggered, this, &MainWindow::onEditContact);
    connect(addToGroupAction, &QAction::triggered, this, &MainWindow::onAddContactToGroup);
    connect(removeFromGroupAction, &QAction::triggered, this, &MainWindow::onRemoveContactFromGroup);
}

void MainWindow::setupTableView()
{
    contactModel = new ContactModel(this);

    proxyModel = new ContactSearchProxyModel(this);
    proxyModel->setSourceModel(contactModel);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->rebuildIndex();

    tableView = new QTableView(this);
    tableView->setModel(proxyModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(true);

    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    tableView->setSortingEnabled(true);

    connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
}

void MainWindow::setupDetailPanel()
{
    detailPanel = new ContactDetailPanel(this);
}

void MainWindow::setupGroupPanel()
{
    groupManager = new ContactGroupManager(this);
    groupManager->setContactModel(contactModel);
    proxyModel->setGroupManager(groupManager);

    QWidget *groupWidget = new QWidget();
    QVBoxLayout *groupLayout = new QVBoxLayout(groupWidget);
    groupLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *groupLabel = new QLabel("分组");

    groupList = new QListWidget();
    groupList->setMinimumWidth(120);
    groupList->setMaximumWidth(180);

    QHBoxLayout *groupButtonsLayout = new QHBoxLayout();
    QPushButton *addGroupBtn = new QPushButton("+");
    QPushButton *deleteGroupBtn = new QPushButton("-");
    groupButtonsLayout->addWidget(addGroupBtn);
    groupButtonsLayout->addWidget(deleteGroupBtn);

    groupLayout->addWidget(groupLabel);
    groupLayout->addWidget(groupList);
    groupLayout->addLayout(groupButtonsLayout);

    groupWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    updateGroupList();

    connect(groupList, &QListWidget::itemClicked,
            this, &MainWindow::onGroupClicked);
    connect(addGroupBtn, &QPushButton::clicked,
            this, &MainWindow::onAddGroup);
    connect(deleteGroupBtn, &QPushButton::clicked,
            this, &MainWindow::onDeleteGroup);
}

void MainWindow::updateGroupList()
{
    groupList->clear();
    QListWidgetItem *allItem = new QListWidgetItem("全部");
    allItem->setData(Qt::UserRole, "全部");
    groupList->addItem(allItem);

    for (const QString &name : groupManager->getAllGroupNames()) {
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, name);
        groupList->addItem(item);
    }

    if (!currentGroup.isEmpty()) {
        for (int i = 0; i < groupList->count(); ++i) {
            QListWidgetItem *item = groupList->item(i);
            if (item->data(Qt::UserRole).toString() == currentGroup) {
                item->setSelected(true);
                break;
            }
        }
    } else {
        groupList->item(0)->setSelected(true);
        currentGroup = "全部";
    }
}

void MainWindow::onAddContact()
{
    ContactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact contact = dialog.getContact();
        contactModel->addContact(contact);
        int row = contactModel->rowCount() - 1;
        proxyModel->addContactIndex(row);
        currentGroup = "全部";
        proxyModel->setCurrentGroup(currentGroup);
        groupList->clearSelection();
    }
}

void MainWindow::onDeleteContact()
{
    QModelIndexList selected = tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择要删除的联系人");
        return;
    }

    QModelIndex proxyIndex = selected.first();
    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();
    Contact contact = contactModel->getContact(row);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该联系人吗？",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        groupManager->removeContactFromAllGroups(contact.id);
        contactModel->removeContact(row);
        proxyModel->rebuildIndex();
        detailPanel->updateContact(Contact());
    }
}

void MainWindow::onEditContact()
{
    QModelIndexList selected = tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择要编辑的联系人");
        return;
    }

    QModelIndex proxyIndex = selected.first();
    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();
    Contact contact = contactModel->getContact(row);

    ContactDialog dialog(contact, this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact updated = dialog.getContact();
        contactModel->updateContact(row, updated);
        proxyModel->updateContactIndex(row);
        detailPanel->updateContact(updated);
    }
}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if (selected.isEmpty()) {
        detailPanel->updateContact(Contact());
        return;
    }

    QModelIndex proxyIndex = selected.indexes().first();
    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    Contact contact = contactModel->getContact(sourceIndex.row());
    detailPanel->updateContact(contact);
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    proxyModel->setSearchKeyword(text);
}

void MainWindow::onGroupClicked(QListWidgetItem *item)
{
    currentGroup = item->data(Qt::UserRole).toString();
    proxyModel->setCurrentGroup(currentGroup);
}

void MainWindow::onAddGroup()
{
    bool ok;
    QString name = QInputDialog::getText(this, "新建分组", "输入分组名称:",
                                         QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        if (groupManager->createGroup(name)) {
            updateGroupList();
        } else {
            QMessageBox::warning(this, "提示", "分组已存在");
        }
    }
}

void MainWindow::onDeleteGroup()
{
    QListWidgetItem *selected = groupList->currentItem();
    if (!selected) return;

    QString name = selected->data(Qt::UserRole).toString();
    if (name == "全部") {
        QMessageBox::warning(this, "提示", "不能删除\"全部\"分组");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该分组吗？",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        groupManager->deleteGroup(name);
        if (currentGroup == name) {
            currentGroup = "全部";
            proxyModel->setCurrentGroup("全部");
        }
        updateGroupList();
    }
}

void MainWindow::onAddContactToGroup()
{
    QListWidgetItem *selectedGroup = groupList->currentItem();
    if (!selectedGroup) {
        QMessageBox::information(this, "提示", "请先选择一个分组");
        return;
    }

    QString groupName = selectedGroup->data(Qt::UserRole).toString();
    if (groupName == "全部") {
        QMessageBox::information(this, "提示", "不能添加到\"全部\"分组");
        return;
    }

    ContactSelectionDialog dialog(contactModel, groupManager,
                                  "选择要添加到 \"" + groupName + "\" 的联系人",
                                  groupName, false, this);

    if (dialog.exec() == QDialog::Accepted) {
        QList<Contact> selectedContacts = dialog.getSelectedContacts();
        foreach (const Contact &c, selectedContacts) {
            groupManager->addContactToGroup(c.id, groupName);
        }
        proxyModel->refreshFilter();
    }
}

void MainWindow::onRemoveContactFromGroup()
{
    QListWidgetItem *selectedGroup = groupList->currentItem();
    if (!selectedGroup) {
        QMessageBox::information(this, "提示", "请先选择一个分组");
        return;
    }

    QString groupName = selectedGroup->data(Qt::UserRole).toString();
    if (groupName == "全部") {
        QMessageBox::information(this, "提示", "不能从\"全部\"分组移除");
        return;
    }

    ContactSelectionDialog dialog(contactModel, groupManager,
                                  "选择要从 \"" + groupName + "\" 移除的联系人",
                                  groupName, true, this);

    if (dialog.exec() == QDialog::Accepted) {
        QList<Contact> selectedContacts = dialog.getSelectedContacts();
        foreach (const Contact &c, selectedContacts) {
            groupManager->removeContactFromGroup(c.id, groupName);
        }
        proxyModel->refreshFilter();
    }
}
