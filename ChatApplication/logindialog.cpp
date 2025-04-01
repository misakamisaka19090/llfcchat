#include "logindialog.h"
#include "ui_logindialog.h"

/******************************************************************************
 *
 * @file       logindialog.cpp
 * @brief      LoginDialog 类的实现文件
 *
 * @details    该文件实现了 LoginDialog 类的构造函数和析构函数，
 *             并设置了 "注册" 按钮点击时的信号与槽机制，用于触发界面切换。
 *
 * @author     misaka
 * @date       2025/03/27
 *****************************************************************************/

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
