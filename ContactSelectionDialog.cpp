#include "ContactSelectionDialog.h"
#include <QHeaderView>
#include <QAbstractItemModel>
#include "ContactGroupManager.h"

ContactSelectionDialog::ContactSelectionDialog(ContactModel *model, ContactGroupManager *manager,
                                               const QString &title, const QString &groupName,
                                               bool showInGroup, QWidget *parent)
    : QDialog(parent), contactModel(model), groupManager(manager),
      groupName(groupName), showInGroup(showInGroup)
{
    setWindowTitle(title);
    setMinimumSize(600, 400);
    setupUI();
}

void ContactSelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    dialogProxyModel = new ContactSearchProxyModel(this);
    dialogProxyModel->setSourceModel(contactModel);
    dialogProxyModel->setGroupManager(groupManager);
    dialogProxyModel->rebuildIndex();

    if (showInGroup) {
        dialogProxyModel->setCurrentGroup(groupName);
    } else {
        dialogProxyModel->setCurrentGroup("全部");
    }

    tableView = new QTableView(this);
    tableView->setModel(dialogProxyModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *selectAllBtn = new QPushButton("全选", this);
    QPushButton *deselectAllBtn = new QPushButton("取消全选", this);
    QPushButton *okBtn = new QPushButton("确定", this);
    QPushButton *cancelBtn = new QPushButton("取消", this);

    buttonLayout->addWidget(selectAllBtn);
    buttonLayout->addWidget(deselectAllBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    mainLayout->addWidget(tableView);
    mainLayout->addLayout(buttonLayout);

    connect(selectAllBtn, &QPushButton::clicked, this, &ContactSelectionDialog::onSelectAll);
    connect(deselectAllBtn, &QPushButton::clicked, this, &ContactSelectionDialog::onDeselectAll);
    connect(okBtn, &QPushButton::clicked, this, &ContactSelectionDialog::onConfirm);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void ContactSelectionDialog::onSelectAll()
{
    QAbstractItemModel *model = tableView->model();
    if (model) {
        QItemSelection selection(model->index(0, 0), 
                                model->index(model->rowCount() - 1, model->columnCount() - 1));
        tableView->selectionModel()->select(selection, QItemSelectionModel::Select);
        tableView->setFocus();
        tableView->viewport()->update();
    }
}

void ContactSelectionDialog::onDeselectAll()
{
    tableView->clearSelection();
    tableView->viewport()->update();
}

void ContactSelectionDialog::onConfirm()
{
    accept();
}

QList<Contact> ContactSelectionDialog::getSelectedContacts() const
{
    QList<Contact> contacts;
    QModelIndexList selected = tableView->selectionModel()->selectedRows();

    foreach (const QModelIndex &proxyIndex, selected) {
        QModelIndex sourceIndex = dialogProxyModel->mapToSource(proxyIndex);
        contacts.append(contactModel->getContact(sourceIndex.row()));
    }

    return contacts;
}