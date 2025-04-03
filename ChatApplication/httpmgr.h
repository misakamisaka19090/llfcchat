#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QUrl>
#include <memory>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

#include "global.h"
#include "singleton.h"

/**
 * @class HttpMgr
 * @brief HTTP管理类，用于管理与网络请求相关的所有操作。
 *
 * HttpMgr 是单例模式的实现，提供全局唯一的网络管理实例，用于处理HTTP请求。
 * 它基于Qt的 `QNetworkAccessManager`，用于发送和接收网络请求，确保应用中的网络操作一致且集中管理。
 *
 * 通过继承 `Singleton<HttpMgr>` 和 `QObject`，该类可以作为单例对象使用，并结合Qt的信号槽机制，
 * 发送网络请求完成的信号，方便其他模块与其通信。
 */
class HttpMgr : public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    /**
     * @brief 析构函数
     *
     * 公有析构函数，确保智能指针在释放对象时正确调用析构函数。
     */
    ~HttpMgr();

    /**
     * @brief 发送POST HTTP请求的函数
     *
     * @param url 目标URL
     * @param json 需要发送的JSON对象
     * @param req_id 请求的ID，用于标识不同的请求
     * @param mod 模块信息，用于区分请求来源
     *
     * 该函数负责发送POST请求，将指定的JSON数据发送到给定的URL，并通过模块和请求ID进行区分。
     * 成功或失败时通过信号通知其他组件处理。
     */
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
    /**
     * @brief 私有构造函数
     *
     * 确保HttpMgr类只能通过 `Singleton<HttpMgr>::GetInstance()` 获取实例，
     * 避免在程序中创建多个网络管理器实例，确保网络请求集中管理。
     */
    friend class Singleton<HttpMgr>;
    HttpMgr();

    /**
     * @brief QNetworkAccessManager对象，用于处理HTTP请求。
     *
     * QNetworkAccessManager是Qt内置的网络管理类，它负责管理网络请求的发送与接收，
     * 通过它可以方便地执行GET、POST等HTTP请求，并处理与服务器之间的数据传输。
     */
    QNetworkAccessManager _manager;


private slots:
    // 用来接收sig_http_finish信号
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    /**
     * @brief HTTP请求完成信号
     *
     * 当HTTP请求完成（无论成功或失败）时，发出该信号通知其他模块进行后续操作。
     * 通过信号槽机制，可以方便地实现请求的异步处理，并在请求完成后触发相关逻辑。
     *
     * @param id 请求的ID
     * @param res 返回的响应结果
     * @param err 错误代码（如果有）
     * @param mod 请求的模块信息
     */
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    /**
     * @brief 用户注册模块HTTP请求完成信号
     *
     * 当HTTP请求在用户注册模块中完成时，发出该信号通知其他模块或组件进行后续处理。
     * 该信号传递请求ID、响应结果和错误代码，方便相关模块根据请求的状态执行相应操作。
     *
     * @param id 请求的ID，用于区分不同的请求。
     * @param res 返回的响应结果，包含服务器返回的内容。
     * @param err 错误代码，指示请求是否成功。如果请求失败，返回相应的错误代码。
     */
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
