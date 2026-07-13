#include "mainwindow.h"
#include <QVBoxLayout>
#include <QAction>
#include <QStyle>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("个人通讯录 V1.0");
    resize(900, 600);
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupToolBar();
    setupTableView();
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
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setCentralWidget(tableView);
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
    }
}