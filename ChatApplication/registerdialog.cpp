#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // 设置 password_Edit 和 comfirmPassword_Edit 的显示模式为密码模式，即输入时以“*”显示
    ui->password_Edit->setEchoMode(QLineEdit::Password);
    ui->comfirmPassword_Edit->setEchoMode(QLineEdit::Password);

    // 连接 login_Button 的 clicked 信号到 loginRegister 槽函数，用于处理注册逻辑
    connect(ui->login_Button, &QPushButton::clicked, this, &RegisterDialog::loginRegister);

    // 设置 err_tip 的属性为"normal"，用于样式区分
    ui->err_tip->setProperty("state", "normal");

    // 应用 err_tip 的样式 确保属性变更生效
    repolish(ui->err_tip);

	connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
}

RegisterDialog::~RegisterDialog()
{
	delete ui;
}

void RegisterDialog::slot_reg_mod_finish(ReqID id, QString res, ErrorCodes err)
{
	if (err != ErrorCodes::SUCCESS) {
        //错误显示是随便应付客户的
		showTip(tr("网络请求错误"), false);
		return;
	}

	// 解析 JSON 字符串,res需转化为QByteArray
	QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
	if (jsonDoc.isNull()) {
		showTip(tr("json解析错误"), false);
		return;
	}

	//这里是转化失败 无法转化为json对象。
	if (!jsonDoc.isObject()) {
		showTip(tr("json解析错误"), false);
		return;
	}

	QJsonObject jsonObj = jsonDoc.object();

	//调用对应的逻辑
	return;
}


// 槽函数，当点击获取验证码按钮时触发，用于处理验证码逻辑
void RegisterDialog::on_getVerify_Button_clicked()
{
    // 获取用户输入的邮箱地址
    auto email = ui->email_Edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();

    if (match) {
        // 如果邮箱格式正确，发送 HTTP 请求以获取验证码
        // (此处应当编写发送验证码的逻辑)
    } else {
        // 如果邮箱格式不正确，显示提示信息，通知用户邮箱输入有误
        showTip(tr("邮箱地址不正确"), false);

    }
}

// 用于显示提示信息的函数
void RegisterDialog::showTip(QString str, bool b_ok)
{
    // 判断是否为正常提示
    if (b_ok) {
        ui->err_tip->setProperty("state", "normal"); // 设置为正常状态
    }
    else {
        ui->err_tip->setProperty("state", "err"); // 设置为错误状态
    }

    // 设置提示信息文本
    ui->err_tip->setText(str);

    // 确保提示样式的属性变更生效
    repolish(ui->err_tip);
}
