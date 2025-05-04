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

/**
 * @brief TcpMgr 类用于管理 TCP 网络通信，处理服务器连接、数据收发以及消息分发。
 *        该类使用单例模式，确保系统中仅有一个 TCP 管理器。
 */
class TcpMgr : public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    /**
     * @brief 析构函数，释放资源
     */
    ~TcpMgr();

private:
    // 允许 Singleton 访问构造函数
    friend class Singleton<TcpMgr>;

    /**
     * @brief 构造函数，初始化 TCP 套接字、信号槽连接和消息处理器
     */
    TcpMgr();

    /**
     * @brief 初始化消息处理函数映射，根据不同的消息 ID 设置对应的处理逻辑
     */
    void initHandlers();

    /**
     * @brief 处理接收到的消息，根据 ReqId 查找对应处理函数进行处理
     * @param id 消息 ID（对应请求或响应类型）
     * @param len 消息体长度
     * @param data 消息体内容
     */
    void handleMsg(ReqId id, int len, QByteArray data);

    QTcpSocket _socket;  // TCP 套接字对象，用于发送和接收数据
    QString _host;       // 要连接的服务器主机地址
    uint16_t _port;      // 要连接的服务器端口
    QByteArray _buffer;  // 接收数据缓冲区，用于处理粘包问题

    bool _b_recv_pending;  // 当前是否处于等待接收完整消息状态
    quint16 _message_id;   // 当前解析中的消息 ID
    quint16 _message_len;  // 当前解析中的消息长度

    /**
     * @brief 消息处理器映射表，键为 ReqId，值为对应的处理函数
     */
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;

public slots:
    /**
     * @brief 槽函数：连接到服务器
     * @param info 包含服务器地址和端口信息
     */
    void slot_tcp_connect(ServerInfo info);

    /**
     * @brief 槽函数：发送数据到服务器
     * @param reqId 请求 ID（消息类型）
     * @param data 要发送的字节数据
     */
    void slot_send_data(ReqId reqId, QByteArray data);

signals:
    /**
     * @brief 信号：连接是否成功
     * @param bsuccess true 表示连接成功，false 表示失败
     */
    void sig_con_success(bool bsuccess);

    /**
     * @brief 信号：发送数据（供外部触发发送）
     */
    void sig_send_data(ReqId reqId, QByteArray data);

    /**
     * @brief 信号：登录成功后切换到聊天窗口
     */
    void sig_swich_chatdlg();

    /**
     * @brief 信号：加载申请列表
     * @param json_array JSON 数组格式的好友申请列表
     */
    void sig_load_apply_list(QJsonArray json_array);

    /**
     * @brief 信号：登录失败
     * @param 错误码（见 ErrorCodes 定义）
     */
    void sig_login_failed(int);

    /**
     * @brief 信号：搜索用户结果
     * @param 包含搜索结果的结构体指针，为空表示失败
     */
    void sig_user_search(std::shared_ptr<SearchInfo>);

    /**
     * @brief 信号：收到添加好友请求通知
     * @param 申请信息结构体
     */
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);

    /**
     * @brief 信号：收到验证好友请求通知
     * @param 验证信息结构体
     */
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);

    /**
     * @brief 信号：验证好友请求响应
     * @param 验证响应信息结构体
     */
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);

    /**
     * @brief 信号：收到文字聊天消息
     * @param 消息结构体指针
     */
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};

#endif // TCPMGR_H
