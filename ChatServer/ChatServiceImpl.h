#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <mutex>
#include "data.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::ChatService;
using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;


// ChatServiceImpl 类：继承 ChatService::Service，是 ChatService 的服务端实现类
class ChatServiceImpl final : public ChatService::Service
{
public:
    // 构造函数
    ChatServiceImpl();

    // 重写的 gRPC 接口：通知添加好友（由客户端调用）
    Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request,
        AddFriendRsp* reply) override;

    // 重写的 gRPC 接口：通知验证好友（由客户端调用）
    Status NotifyAuthFriend(ServerContext* context,
        const AuthFriendReq* request, AuthFriendRsp* response) override;

    // 重写的 gRPC 接口：通知文本聊天消息（发送聊天内容）
    Status NotifyTextChatMsg(::grpc::ServerContext* context,
        const TextChatMsgReq* request, TextChatMsgRsp* response) override;

    // 获取用户基本信息，返回 true 表示获取成功
    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);

private:
};

