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

    // 应用 err_tip 的样式，确保属性变更生效
    repolish(ui->err_tip);

    // 连接 HTTP 管理器的注册信号至槽函数
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    // 初始化 HTTP 回调函数
    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}


// 槽函数，当点击获取验证码按钮时触发，用于处理验证码逻辑
void RegisterDialog::on_getVerify_Button_clicked()
{
    // 获取用户输入的邮箱地址
    auto email = ui->email_Edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); // 使用正则表达式验证邮箱格式
    bool match = regex.match(email).hasMatch();

    if (match) {
        // 如果邮箱格式正确，发送 HTTP 请求以获取验证码
        // 此处编写发送验证码的逻辑
    } else {
        // 如果邮箱格式不正确，显示提示信息，通知用户邮箱输入有误
        showTip(tr("邮箱地址不正确"), false);
    }
}

// 用于显示提示信息的函数
void RegisterDialog::showTip(QString str, bool b_ok)
{
    // 根据 b_ok 设置提示信息的状态属性
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

// 槽函数，处理注册请求结果
void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        //错误显示是随便应付客户的
        showTip(tr("网络请求错误"), false);
        return;
    }

    // 解析服务器返回的 JSON 字符串，res 需转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    // 判断是否成功解析为 JSON 对象
    if (!jsonDoc.isObject()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    // 根据请求 ID 调用对应的处理函数
    _handlers[id](jsonDoc.object());

    return;
}

// 初始化 HTTP 回调函数
void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回包的处理逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug()<< "email is " << email ;
    });
}
