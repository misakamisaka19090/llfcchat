#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

/**
 * @brief AsioIOServicePool 类
 *
 * 该类用于管理多个 io_context 实例，构建一个基于 Boost.Asio 的 I/O 线程池。
 * 每个 io_context 实例都由一个单独的线程运行，并通过轮询的方式将异步任务分配给不同的线程。
 * 使用 executor_work_guard 确保 io_context 在没有任务时不会退出，保持线程池活跃。
 *
 * 主要功能实现：
 *
 * 函数：GetIOService()
 * - 使用轮询方式返回一个 io_context 实例，每次调用返回不同的实例。
 *
 * 函数：Stop()
 * - 停止所有 io_context 实例，并等待所有工作线程完成任务并退出。
 *
 * 数据成员：
 * - `_ioServices`：存储用于执行异步 I/O 任务的多个 io_context 实例。
 * - `_workGuards`：存储为每个 io_context 保持活跃状态的 executor_work_guard
 * - `_threads`：存储工作线程，负责运行各自的 io_context::run()。
 * - `_nextIOService`：记录当前使用的 io_context 索引，指示下一个将被分配的 io_context。
 */
class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    using WorkGuard = boost::asio::executor_work_guard<IOService::executor_type>;
    using WorkGuardPtr = std::unique_ptr<WorkGuard>;

    //没有构造函数和拷贝构造
    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    /**
     * @brief 析构函数
     * 析构函数会停止所有 io_context，并等待所有工作线程完成。
     */
    ~AsioIOServicePool();

    /**
     * @brief 获取一个 io_context 实例（用于提交异步任务）
     * 采用轮询（Round-Robin）方式返回一个 io_context 的引用，以实现负载均衡。
     * @return boost::asio::io_context& 可用于提交任务的 io_context。
     */
    boost::asio::io_context& GetIOService();

    /**
     * @brief 停止所有 io_context 并退出线程池
     * 1. 调用 stop() 停止所有 io_context 实例，终止其事件循环。
     * 2. 重置所有 executor_work_guard，允许 io_context 安全退出。
     * 3. 等待线程池中所有线程执行完毕并退出。
     * 注即使 executor_work_guard 被 reset，如果 io_context 中仍有挂起的操作，也需要手动调用 stop() 才能确保 run() 退出。
     */
    void Stop();

private:
    /**
     * @brief 构造函数
     *
     * 初始化指定数量的 io_context，每个 io_context 配一个 executor_work_guard 和工作线程。
     * 线程池立即启动，每个线程内部运行对应 io_context 的 run() 方法。
     *
     * @param size 指定 io_context 与线程的数量
     */
    AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);

    // 存储 io_context 实例
    std::vector<IOService> _ioServices;

    // 存储每个 io_context 的 executor_work_guard，防止其退出
    std::vector<WorkGuardPtr> _workGuards;

    // 存储工作线程，负责运行各自的 io_context::run()
    std::vector<std::thread> _threads;

    // 记录当前使用的 io_context 索引，采用轮询方式返回 io_context
    std::size_t _nextIOService;
};
