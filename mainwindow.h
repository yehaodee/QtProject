#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include <QSplitter>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include "ContactModel.h"
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

private:
    void setupUI();
    void setupToolBar();
    void setupTableView();
    void setupDetailPanel();

    ContactModel *contactModel;
    QSortFilterProxyModel *proxyModel;
    QTableView *tableView;
    ContactDetailPanel *detailPanel;
    QSplitter *splitter;
};

#endif // MAINWINDOW_H