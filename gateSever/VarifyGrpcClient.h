#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

// gRPC 连接池类
// 该类用于管理 gRPC 连接池，确保多个请求共享一定数量的连接。
// 它会为每个请求提供一个连接，并在处理完成后将连接返回池中，以便其他请求重用。
class RPConPool {
public:
	RPConPool(size_t poolSize, std::string host, std::string port);
	~RPConPool();
	std::unique_ptr<VarifyService::Stub> getConnection();
	void returnConnection(std::unique_ptr<VarifyService::Stub> context);
	void Close();
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


// 验证码 gRPC 客户端类
// 该类通过连接池获取 gRPC 连接，并实现获取验证码的操作。
class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;

public:
    /**
     * @brief 获取验证码。
     *
     * 通过调用 gRPC 服务的 GetVarifyCode 方法，获取指定邮箱的验证码。
     *
     * @param email 用户的邮箱地址。
     * @return 返回 gRPC 的响应结果，其中包含验证码信息或错误信息。
     */
    GetVarifyRsp GetVarifyCode(std::string email);

private:
    /**
     * @brief 构造函数，初始化客户端并创建连接池。
     */
    VerifyGrpcClient();

    // 连接池对象，用于管理与 gRPC 服务的连接
    std::unique_ptr<RPConPool> pool_;
};