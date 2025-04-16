#include "mainwindow.h"
#include "ui_mainwindow.h"

// MainWindow 类的构造函数
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)  // 调用父类构造函数
    , ui(new Ui::MainWindow)  // 初始化 UI 指针
{
    ui->setupUi(this);  // 设置 UI

    // 创建 LoginDialog 实例和 RegisterDialog 实例
    _login_dlg = new LoginDialog();  // 创建新的 LoginDialog 实例
    _register_dlg = new RegisterDialog();  // 创建新的 RegisterDialog 实例
    _reset_dlg = new ResetDialog();

    // 创建一个 QStackedWidget 并添加界面
    _stackedWidget = new QStackedWidget(this);  // 创建 QStackedWidget 实例
    _stackedWidget->addWidget(_login_dlg);  // 将登录对话框添加到 QStackedWidget
    _stackedWidget->addWidget(_register_dlg);  // 将注册对话框添加到 QStackedWidget
    _stackedWidget->addWidget(_reset_dlg);
    setCentralWidget(_stackedWidget);  // 设置 QStackedWidget 为中央控件

    //连接 注册界面信号 与 注册界面函数
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);

    //连接 登录界面信号 与 登录界面函数
    connect(_register_dlg, &RegisterDialog::switchLogin, this, &MainWindow::SlotSwitchLogin);
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin);

    //连接 重置密码界面信号 与 转化重置密码界面函数
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

// MainWindow 类的析构函数
MainWindow::~MainWindow()
{
    delete ui;  // 删除 UI 对象以防止内存泄漏
    delete _login_dlg;  // 删除 LoginDialog 对象
    delete _register_dlg;  // 删除 RegisterDialog 对象
    delete _reset_dlg;
    delete _stackedWidget;  // 删除 QStackedWidget 对象
}

// 切换到注册界面
void MainWindow::SlotSwitchRegister() {
    _stackedWidget->setCurrentWidget(_register_dlg);  // 切换到注册对话框
}

//从注册界面返回登录界面
void MainWindow::SlotSwitchLogin() {
    _stackedWidget->setCurrentWidget(_login_dlg);  // 切换到登录对话框
}

// 切换到重置密码界面
void MainWindow::SlotSwitchReset() {
    _stackedWidget->setCurrentWidget(_reset_dlg);  // 切换到重置密码对话框
}
