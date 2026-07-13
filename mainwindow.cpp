#include "mainwindow.h"
#include <QVBoxLayout>
#include <QAction>
#include <QStyle>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("个人通讯录 V1.0");
    resize(1500, 800);
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupToolBar();
    setupTableView();
    setupDetailPanel();

    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(tableView);
    splitter->addWidget(detailPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    setCentralWidget(splitter);
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

    tableView = new QTableView(this);
    tableView->setModel(contactModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(true);

    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

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
    }
}

void MainWindow::onEditContact()
{
    QModelIndexList selected = tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择要编辑的联系人");
        return;
    }

    int row = selected.first().row();
    Contact contact = contactModel->getContact(row);

    ContactDialog dialog(contact, this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact updated = dialog.getContact();
        contactModel->updateContact(row, updated);
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

    int row = selected.first().row();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该联系人吗？",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        contactModel->removeContact(row);
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

    QModelIndex index = selected.indexes().first();
    Contact contact = contactModel->getContact(index.row());
    detailPanel->updateContact(contact);
}
