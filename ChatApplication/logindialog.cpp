#include "logindialog.h"
#include "ui_logindialog.h"


LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    // 初始化 UI 组件
    ui->setupUi(this);

    // 连接信号与槽机制
    // 点击 "注册" 按钮时，触发 switchRegister 信号
    connect(ui->register_Button, &QPushButton::clicked, this, &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
