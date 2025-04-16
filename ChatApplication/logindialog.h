#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

    void slot_forget_pwd();
signals:
    // 信号：switchRegister 用于切换到注册界面
    void switchRegister();

    // 信号：switchReset 用于切换到重置密码界面
    void switchReset();
};

#endif // LOGINDIALOG_H
