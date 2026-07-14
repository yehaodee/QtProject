#ifndef CONTACTSELECTIONDIALOG_H
#define CONTACTSELECTIONDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "ContactModel.h"
#include "ContactSearchProxyModel.h"

class ContactSelectionDialog : public QDialog {
    Q_OBJECT
public:
    ContactSelectionDialog(ContactModel *model, ContactGroupManager *groupManager,
                           const QString &title, const QString &groupName = "",
                           bool showInGroup = false, QWidget *parent = nullptr);
    
    QList<Contact> getSelectedContacts() const;

private slots:
    void onSelectAll();
    void onDeselectAll();
    void onConfirm();

private:
    void setupUI();

    ContactModel *contactModel;
    ContactGroupManager *groupManager;
    ContactSearchProxyModel *dialogProxyModel;
    QTableView *tableView;
    QString groupName;
    bool showInGroup;
};

#endif // CONTACTSELECTIONDIALOG_H