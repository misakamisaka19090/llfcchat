#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resetdialog.h"
#include "tcpmgr.h"
#include <QLayout>

// MainWindow 类的构造函数
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)  // 调用父类构造函数
    , ui(new Ui::MainWindow)  // 初始化 UI 指针
{
    ui->setupUi(this);  // 设置 UI

    // 创建 LoginDialog、RegisterDialog、ResetDialog 实例
    _login_dlg = new LoginDialog();         // 登录界面
    _register_dlg = new RegisterDialog();   // 注册界面
    _reset_dlg = new ResetDialog();         // 重置密码界面

    // 创建 QStackedWidget 容器，用于页面切换
    _stackedWidget = new QStackedWidget(this);
    _stackedWidget->addWidget(_login_dlg);      // 添加登录页
    _stackedWidget->addWidget(_register_dlg);   // 添加注册页
    _stackedWidget->addWidget(_reset_dlg);      // 添加重置密码页

    setCentralWidget(_stackedWidget);  // 设置为主窗口中央控件

    // 信号连接：登录页 -> 切换到注册页
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);

    // 信号连接：注册页/重置页 -> 切换回登录页
    connect(_register_dlg, &RegisterDialog::switchLogin, this, &MainWindow::SlotSwitchLogin);
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin);

    // 信号连接：登录页 -> 切换到重置密码页
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

    //连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_swich_chatdlg, this, &MainWindow::SlotSwitchChat);


    //测试
    //emit TcpMgr::GetInstance()->sig_swich_chatdlg();
}

// MainWindow 析构函数
MainWindow::~MainWindow()
{
    delete ui;
    delete _login_dlg;
    delete _register_dlg;
    delete _reset_dlg;
    delete _stackedWidget;
}

// 槽函数：切换到注册页面
void MainWindow::SlotSwitchRegister() {
    _stackedWidget->setCurrentWidget(_register_dlg);
}

// 槽函数：切换到登录页面
void MainWindow::SlotSwitchLogin() {
    _stackedWidget->setCurrentWidget(_login_dlg);
}

// 槽函数：切换到重置密码页面
void MainWindow::SlotSwitchReset() {
    _stackedWidget->setCurrentWidget(_reset_dlg);
}


void MainWindow::SlotSwitchChat() {
    // 如果聊天窗口未创建，则创建一个新窗口
    if (!_chat_dlg) {
        _chat_dlg = new ChatDialog();
        _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    }

    // 设置中央窗口为聊天界面并显示
    setCentralWidget(_chat_dlg);
    _chat_dlg->show();

    // 隐藏 _stackedWidget（包括登录、注册和重置密码页面）
    _stackedWidget->hide();

    // 设置主窗口大小限制
    setMinimumSize(1050, 900);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

