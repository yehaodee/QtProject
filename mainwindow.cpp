#include "mainwindow.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QAction>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("实用的小型通讯录");
    resize(900, 600);
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupToolBar();

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // 左侧：联系人列表
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *listTitle = new QLabel("联系人列表", this);
    listTitle->setStyleSheet("font-size: 14px; font-weight: bold; padding: 8px;");
    contactList = new QListWidget(this);
    contactList->setAlternatingRowColors(true);

    leftLayout->addWidget(listTitle);
    leftLayout->addWidget(contactList);
    leftPanel->setMinimumWidth(220);

    // 右侧：详细信息
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(12, 12, 12, 12);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(64, 64);
    avatarLabel->setStyleSheet(
        "background-color: #4A90D9; color: white; font-size: 28px;"
        "font-weight: bold; border-radius: 32px;");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setText("?");

    nameTitle = new QLabel("未选择联系人", this);
    nameTitle->setStyleSheet("font-size: 22px; font-weight: bold;");

    headerLayout->addWidget(avatarLabel);
    headerLayout->addSpacing(12);
    headerLayout->addWidget(nameTitle);
    headerLayout->addStretch();

    QGroupBox *detailGroup = new QGroupBox("详细信息", this);
    QFormLayout *formLayout = new QFormLayout(detailGroup);
    formLayout->setSpacing(10);

    nameLabel = new QLabel("--", this);
    phoneLabel = new QLabel("--", this);
    emailLabel = new QLabel("--", this);
    addressLabel = new QLabel("--", this);
    notesLabel = new QLabel("--", this);

    nameLabel->setWordWrap(true);
    phoneLabel->setWordWrap(true);
    emailLabel->setWordWrap(true);
    addressLabel->setWordWrap(true);
    notesLabel->setWordWrap(true);

    formLayout->addRow("姓  名:", nameLabel);
    formLayout->addRow("电  话:", phoneLabel);
    formLayout->addRow("邮  箱:", emailLabel);
    formLayout->addRow("地  址:", addressLabel);
    formLayout->addRow("备  注:", notesLabel);

    rightLayout->addLayout(headerLayout);
    rightLayout->addWidget(detailGroup);
    rightLayout->addStretch();
    rightPanel->setMinimumWidth(350);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);
}

/*
    顶部工具栏
    包含一些软件的核心操作按钮
    如新建联系人、删除联系人、保存修改等
*/
void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("核心操作");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "新建联系人");
    toolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), "删除联系人");
    toolBar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "保存修改");
}