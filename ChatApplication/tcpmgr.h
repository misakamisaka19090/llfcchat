#ifndef TCPMGR_H
#define TCPMGR_H

#include "global.h"
#include "singleton.h"
#include "userdata.h"

#include <QObject>
#include <QTcpSocket>
#include <functional>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QAbstractSocket>


class TcpMgr:public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    ~ TcpMgr();
private:
    friend class Singleton<TcpMgr>;
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    QTcpSocket _socket;                   // TCP 套接字，用于网络通信
    QString _host;                        // 服务器主机名/IP
    uint16_t _port;                       // 服务器端口
    QByteArray _buffer;                   // 接收数据的缓冲区
    bool _b_recv_pending;                // 是否还在等待接收完整消息
    quint16 _message_id;                 // 当前接收消息的 ID
    quint16 _message_len;                // 当前接收消息的长度
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers; // 消息处理器映射
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);
signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_swich_chatdlg();
    void sig_load_apply_list(QJsonArray json_array);
    void sig_login_failed(int);
    /*
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
    */
};

#endif // TCPMGR_H
