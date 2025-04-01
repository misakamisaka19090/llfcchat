#include "httpmgr.h"

HttpMgr::~HttpMgr() {
}

HttpMgr::HttpMgr()
{
    // 连接了 `sig_http_finish` 信号和 `slot_http_finish` 槽函数
    // 确保网络请求完成后，通过信号槽机制通知相关组件处理响应结果。
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}

/**
 * @brief 发送HTTP POST请求
 *
 * 该函数负责向指定的URL发送一个带有JSON数据的POST请求。请求通过 `QNetworkAccessManager` 发送，
 * 并且在请求完成后，通过信号槽机制通知调用者请求的状态和结果。
 *
 * @param url 请求的目标URL。
 * @param json 需要发送的JSON对象。
 * @param req_id 请求的ID，用于区分不同的请求。
 * @param mod 请求所属的模块，用于在信号中传递模块信息。
 */
void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    // 创建一个HTTP POST请求，并设置请求头和请求体
    QByteArray data = QJsonDocument(json).toJson(); // 将JSON对象转换为字节数组
    QNetworkRequest request(url); // 根据URL创建网络请求
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // 设置请求的内容类型为JSON
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length())); // 设置内容长度

    // 发送请求，并处理响应, 获取自己的智能指针，构造伪闭包并增加智能指针引用计数
    auto self = shared_from_this(); // 获取当前对象的shared_ptr，防止在异步操作完成前对象被销毁
    QNetworkReply * reply = _manager.post(request, data); // 发送POST请求

    // 设置信号和槽等待请求完成
    QObject::connect(reply, &QNetworkReply::finished, [reply, self, req_id, mod](){
        // 处理错误的情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString(); // 打印错误信息
            // 发送信号通知请求失败
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater(); // 清理响应对象
            return;
        }

        // 如果无错误，读取服务器返回的响应数据
        QString res = reply->readAll();

        // 发送信号通知请求成功
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater(); // 清理响应对象
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if (mod == Modules::REGISTERMOD) {
        //发送信号通知指定模块http响应结束
        emit sig_reg_mod_finish(id, res, err);
    }
}
