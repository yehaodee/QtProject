#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include "ContactModel.h"
#include "ContactDialog.h"

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

private:
    void setupUI();
    void setupToolBar();
    void setupTableView();

    ContactModel *contactModel;
    QTableView *tableView;
};

#endif // MAINWINDOW_H