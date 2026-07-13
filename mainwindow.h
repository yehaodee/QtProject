#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include <QSplitter>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QLineEdit>
#include "ContactModel.h"
#include "ContactSearchProxyModel.h"
#include "ContactDialog.h"
#include "ContactDetailPanel.h"

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

private:
    void setupUI();
    void setupToolBar();
    void setupSearchBar();
    void setupTableView();
    void setupDetailPanel();

    ContactModel *contactModel;
    ContactSearchProxyModel *proxyModel;
    QTableView *tableView;
    ContactDetailPanel *detailPanel;
    QSplitter *splitter;
    QLineEdit *searchEdit;
};

#endif // MAINWINDOW_H