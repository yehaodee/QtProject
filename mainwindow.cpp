#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QAction>
#include <QStyle>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("个人通讯录 V1.1");
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

    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addWidget(searchEdit->parentWidget());
    leftLayout->addWidget(tableView);

    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftWidget);
    splitter->addWidget(detailPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    setCentralWidget(splitter);

    connect(searchEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
}

void MainWindow::setupSearchBar()
{
    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *searchLabel = new QLabel("搜索:", searchWidget);
    searchEdit = new QLineEdit(searchWidget);
    searchEdit->setPlaceholderText("输入姓名或电话...");
    searchEdit->setMaximumHeight(30);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);

    searchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchWidget->setMaximumHeight(40);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("核心操作");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *addAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "新建");
    QAction *deleteAction = toolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), "删除");
    QAction *editAction = toolBar->addAction(style()->standardIcon(QStyle::SP_ArrowBack), "编辑");

    connect(addAction, &QAction::triggered, this, &MainWindow::onAddContact);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteContact);
    connect(editAction, &QAction::triggered, this, &MainWindow::onEditContact);
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

void MainWindow::onAddContact()
{
    ContactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact contact = dialog.getContact();
        contactModel->addContact(contact);
        int row = contactModel->rowCount() - 1;
        proxyModel->addContactIndex(row);
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
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该联系人吗？",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        contactModel->removeContact(row);
        proxyModel->rebuildIndex();
        detailPanel->updateContact(Contact());
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
