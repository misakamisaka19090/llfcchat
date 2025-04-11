#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

/**
 * @brief AsioIOServicePool 类
 *
 * 该类用于管理多个 io_context 实例，使用线程池处理异步 I/O 操作。
 * 每个 io_context 实例都由一个单独的线程运行，并通过轮询的方式分配给不同的线程。
 * 该类通过 executor_work_guard 来防止 io_context 在没有任务时退出，确保异步 I/O 操作始终进行。
 *
 * 主要功能实现：
 *
 * 函数：GetIOService()
 * - 使用轮询方式返回一个 io_context 实例，每次调用返回不同的实例。
 *
 * 函数：Stop()
 * - 停止所有 io_context，重置工作守护对象，允许 io_context 正常退出。
 * - 等待所有工作线程完成任务并退出。
 *
 * 数据成员：
 * - `_ioServices`：存储多个 io_context 实例，用于异步 I/O 操作。
 * - `_works`：存储每个 io_context 的 executor_work_guard，防止其退出。
 * - `_threads`：存储工作线程，负责运行各自的 io_context::run()。
 * - `_nextIOService`：记录当前使用的 io_context 索引，采用轮询方式返回 io_context。
 */
class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    // 防止 io_context 在没有任务时退出
    using Work = boost::asio::executor_work_guard<IOService::executor_type>;
    using WorkPtr = std::unique_ptr<Work>;

    /**
     * @brief 析构函数
     *
     * 析构函数会停止所有 io_context，并等待所有工作线程完成。
     */
    ~AsioIOServicePool();

    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    /**
     * @brief 获取一个 io_context 实例
     *
     * 使用轮询方式返回一个 io_context 实例，每次调用返回不同的实例。
     *
     * @return boost::asio::io_context& 返回一个 io_context 引用。
     */
    boost::asio::io_context& GetIOService();

    /**
     * @brief 停止所有 io_context
     *
     * 停止所有 io_context 实例，重置工作守护对象，允许 io_context 正常退出。
     * 并等待所有工作线程完成任务并退出。
     */
    void Stop();

private:
    /**
     * @brief 构造函数
     *
     * 构造一个 AsioIOServicePool 对象，初始化指定数量的 io_context 实例。
     * 默认创建 2 个 io_context 实例，每个实例都由独立的工作线程运行。
     *
     * @param size 创建的 io_context 实例数，默认为 2。
     */
    AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);

    // 存储多个 io_context 实例
    std::vector<IOService> _ioServices;

    // 存储每个 io_context 的 executor_work_guard，防止其退出
    std::vector<WorkPtr> _works;

    // 存储工作线程，负责运行各自的 io_context::run()
    std::vector<std::thread> _threads;

    // 记录当前使用的 io_context 索引，采用轮询方式返回 io_context
    std::size_t _nextIOService;
};
