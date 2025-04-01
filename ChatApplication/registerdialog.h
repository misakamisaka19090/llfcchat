#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "httpmgr.h"


namespace Ui {
class RegisterDialog;
}

/**
 * @class RegisterDialog
 * @brief 注册对话框类，用于用户注册相关功能。
 *
 * 该类继承自QDialog，用于实现用户注册界面的逻辑处理，包括获取验证码、邮箱验证、密码确认等功能。
 */
class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog();

private:
    Ui::RegisterDialog *ui; ///< 界面UI对象指针

signals:
    // 信号：切换到登录界面。
    void loginRegister();

protected slots:
    /**
     * @brief 槽函数：显示提示信息, 在界面上显示错误或成功信息，提示信息包括文本和状态。
     *
     * @param str 提示的文本内容
     * @param b_ok 指示状态，`true`表示正常，`false`表示错误
     */
    void showTip(QString str, bool b_ok);

private slots:
    /**
     * @brief 槽函数：处理点击“获取验证码”按钮的事件。
     * 该函数会验证用户输入的邮箱地址是否合法，并在邮箱格式正确时发送验证码请求。
     */
	void on_getVerify_Button_clicked();

	void slot_reg_mod_finish(ReqID id, QString res, ErrorCodes err);
};

#endif // REGISTERDIALOG_H
