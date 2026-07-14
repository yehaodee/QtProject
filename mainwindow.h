#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include <QSplitter>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include "ContactModel.h"
#include "ContactSearchProxyModel.h"
#include "ContactDialog.h"
#include "ContactDetailPanel.h"
#include "ContactGroupManager.h"
#include "ContactSelectionDialog.h"
#include "DataManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onSearchTextChanged(const QString &text);
    void onGroupClicked(QListWidgetItem *item);
    void onAddGroup();
    void onDeleteGroup();
    void onAddContactToGroup();
    void onRemoveContactFromGroup();

private:
    void setupUI();
    void setupToolBar();
    void setupSearchBar();
    void setupTableView();
    void setupDetailPanel();
    void setupGroupPanel();
    void updateGroupList();

    ContactModel *contactModel;
    ContactSearchProxyModel *proxyModel;
    ContactGroupManager *groupManager;
    DataManager *dataManager;
    QTableView *tableView;
    ContactDetailPanel *detailPanel;
    QSplitter *splitter;
    QLineEdit *searchEdit;
    QListWidget *groupList;
    QString currentGroup;
};

#endif // MAINWINDOW_H