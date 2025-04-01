#include "mainwindow.h"
#include "ui_mainwindow.h"

/******************************************************************************
 *
 * @file       mainwindow.cpp
 * @brief      MainWindow 类的实现文件
 *
 * @details    该文件实现了 MainWindow 类的构造和析构函数，完成了登录与注册对话框的
 *             切换功能，并进行了信号与槽的连接。
 *
 * @author     misaka
 * @date       2025/03/27
 *****************************************************************************/

 // MainWindow 类的构造函数
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)  // 调用父类构造函数
    , ui(new Ui::MainWindow)  // 初始化 UI 指针
{
    ui->setupUi(this);  // 设置 UI

    // 创建 LoginDialog 实例和 RegisterDialog 实例
    _login_dlg = new LoginDialog();  // 创建新的 LoginDialog 实例
    _register_dlg = new RegisterDialog();  // 创建新的 RegisterDialog 实例

    // 创建一个 QStackedWidget 并添加界面
    _stackedWidget = new QStackedWidget(this);  // 创建 QStackedWidget 实例
    _stackedWidget->addWidget(_login_dlg);  // 将登录对话框添加到 QStackedWidget
    _stackedWidget->addWidget(_register_dlg);  // 将注册对话框添加到 QStackedWidget

    setCentralWidget(_stackedWidget);  // 设置 QStackedWidget 为中央控件

    // 连接 LoginDialog 的 switchRegister 信号与 MainWindow 的 SlotSwitchRegister 槽函数
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);

    // 连接 RegisterDialog 的 loginRegister 信号与 MainWindow 的 SlotSwitchLogin 槽函数
    connect(_register_dlg, &RegisterDialog::loginRegister, this, &MainWindow::SlotSwitchLogin);
}

// MainWindow 类的析构函数
MainWindow::~MainWindow()
{
    delete ui;  // 删除 UI 对象以防止内存泄漏
    delete _login_dlg;  // 删除 LoginDialog 对象
    delete _register_dlg;  // 删除 RegisterDialog 对象
    delete _stackedWidget;  // 删除 QStackedWidget 对象
}

// 槽函数：切换到注册界面
void MainWindow::SlotSwitchRegister() {
    _stackedWidget->setCurrentWidget(_register_dlg);  // 切换到注册对话框
}

// 槽函数：切换回登录界面
void MainWindow::SlotSwitchLogin() {
    _stackedWidget->setCurrentWidget(_login_dlg);  // 切换到登录对话框
}
