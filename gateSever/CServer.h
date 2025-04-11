#pragma once

#include "const.h"

/**
 * @brief 服务器类
 *
 * 该类用于创建并管理一个 HTTP 服务器，监听客户端的连接并异步处理请求。
 * 它基于 Boost Asio 库实现异步 I/O 操作，使用 shared_ptr 确保服务器在异步回调中保持存活。
 * 每当通过 `async_accept` 接收到一个客户端连接时，就会创建一个 HttpConnection 实例来处理该连接。
 * 一旦处理完当前连接，服务器会继续监听新的客户端连接，保持服务器持续运行。
 *
 * 主要功能实现：
 *
 * 函数：Start()
 * - 使用 `shared_from_this()` 来确保 `CServer` 对象在异步操作中不会被销毁。
 * - 使用 `async_accept` 异步接收客户端连接，当有新连接时，调用回调函数处理连接。
 * - 如果连接成功，则创建 `HttpConnection` 对象来处理该连接，并调用其 `Start()` 函数开始处理请求。
 * - 每次处理完一个连接后，重新调用 `Start()` 函数继续监听新的连接请求。
 *
 * 数据成员：
 * - `_acceptor`：用于监听客户端连接的 `tcp::acceptor` 对象。
 * - `_ioc`：`io_context` 对象，负责所有的异步 I/O 操作。
 */
class CServer : public std::enable_shared_from_this<CServer>
{
public:
    /**
     * @brief 构造函数
     *
     * 构造一个 CServer 对象，并初始化监听器和 `io_context` 对象。
     * 该构造函数会创建一个 `tcp::acceptor` 对象，用于监听指定端口的客户端连接。
     *
     * @param ioc io_context 对象，用于异步 I/O 操作。
     * @param port 要监听的端口号。
     */
    CServer(boost::asio::io_context& ioc, unsigned short& port);

    /**
     * @brief 启动服务器
     *
     * 启动服务器，开始异步接收客户端连接。
     * 该函数调用 `async_accept` 来异步接收客户端连接，接收到连接后会创建新的 `HttpConnection` 对象来处理请求。
     */
    void Start();

private:

    // 用于监听客户端连接的 acceptor 对象
    tcp::acceptor _acceptor;

    // 用于异步 I/O 操作的 io_context 对象
    boost::asio::io_context& _ioc;
};
