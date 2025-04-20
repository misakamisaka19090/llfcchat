// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
#include "CServer.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"

using namespace std;

// 全局停止标志（未使用，可用于扩展）
bool bstop = false;
// 用于等待退出的条件变量和互斥锁（未使用）
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
    // 读取配置对象单例
    auto& cfg = ConfigMgr::Inst();

    // 获取当前服务器的名称（用于 Redis 标识）
    auto server_name = cfg["SelfServer"]["Name"];

    try {
        // 获取 Asio 线程池实例
        auto pool = AsioIOServicePool::GetInstance();

        // 在 Redis 中设置该服务器的登录人数为 0
        RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, "0");

        // 构造 gRPC 服务地址，例如 "127.0.0.1:50051"
        std::string server_address(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);

        // 创建 gRPC 服务实现类（继承自 protobuf 生成的服务接口）
        ChatServiceImpl service;

        // 创建 gRPC 服务器构建器
        grpc::ServerBuilder builder;

        // 绑定监听地址和不安全凭据（无认证）
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // 注册服务实现
        builder.RegisterService(&service);

        // 构建并启动 gRPC 服务器
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "RPC Server listening on " << server_address << std::endl;

        // 启动一个线程运行 gRPC 服务（阻塞式）
        std::thread grpc_server_thread([&server]() {
            server->Wait(); // 阻塞直到服务器关闭
            });

        // 创建 Boost.Asio 的 IO 上下文
        boost::asio::io_context io_context;

        // 创建信号集，监听 SIGINT 和 SIGTERM（如 Ctrl+C 退出）
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

        // 设置异步回调函数处理退出信号
        signals.async_wait([&io_context, pool, &server](auto, auto) {
            // 收到退出信号后：
            io_context.stop();         // 停止 IO 上下文事件循环
            pool->Stop();              // 停止 Asio 线程池
            server->Shutdown();        // 停止 gRPC 服务
            });

        // 从配置中读取监听的端口号（TCP 聊天服务器）
        auto port_str = cfg["SelfServer"]["Port"];

        // 创建自定义 TCP 聊天服务器对象（传入端口和 io_context）
        CServer s(io_context, atoi(port_str.c_str()));

        // 启动 IO 事件循环（会阻塞直到退出）
        io_context.run();

        // 清理：从 Redis 中删除该服务器的登录状态
        RedisMgr::GetInstance()->HDel(LOGIN_COUNT, server_name);

        // 关闭 Redis 连接
        RedisMgr::GetInstance()->Close();

        // 等待 gRPC 线程退出
        grpc_server_thread.join();
    }
    catch (std::exception& e) {
        // 捕获异常并打印错误信息
        std::cerr << "Exception: " << e.what() << endl;

        // 异常退出时依然做清理工作
        RedisMgr::GetInstance()->HDel(LOGIN_COUNT, server_name);
        RedisMgr::GetInstance()->Close();
    }
}
