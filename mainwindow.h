#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupToolBar();

    QListWidget *contactList;
    QLabel *nameLabel;
    QLabel *phoneLabel;
    QLabel *emailLabel;
    QLabel *addressLabel;
    QLabel *notesLabel;
    QLabel *avatarLabel;
    QLabel *nameTitle;
};

#endif // MAINWINDOW_H