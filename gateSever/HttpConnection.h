#pragma once

#include "const.h"

/**
 * @brief HTTP 连接管理类
 *
 * 该类用于管理一个 HTTP 连接，处理客户端发送的 HTTP 请求，生成并发送响应。
 * 它使用 Boost Asio 库处理异步 I/O 操作，支持 GET 和 POST 请求的处理，
 * 并且通过定时器（`deadline_`）来检查连接的超时。
 *
 * 主要功能实现：
 * - 使用 `http::async_read` 从客户端异步读取 HTTP 请求，实际是在 `Start()` 函数中调用。
 * - 使用 `http::request` 类解析客户端发送的 HTTP 请求。
 * - 使用 `http::response` 类构建 HTTP 响应。
 * - 使用 `WriteResponse` 函数异步发送响应。
 * - 使用 `net::steady_timer` 来处理连接超时检测。
 */
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
    friend class LogicSystem;

public:
    /**
     * @brief 构造函数
     *
     * 构造一个 HttpConnection 对象，并初始化网络连接。
     * 该构造函数调用了 Boost Asio 库中的 `tcp::socket` 来初始化套接字。
     *
     * @param ioc io_context 对象，用于异步 I/O 操作
     */
    HttpConnection(boost::asio::io_context& ioc);

    /**
     * @brief 启动连接
     *
     * 启动连接，开始异步读取请求。调用 `http::async_read` 从客户端异步读取 HTTP 请求数据，
     * 当请求解析成功时，会调用 `HandleReq` 进行请求处理，并通过 `CheckDeadline` 函数启动超时检测。
     */
    void Start();

    /**
     * @brief 获取套接字对象
     *
     * 返回当前连接使用的 TCP 套接字对象，可以用于后续的 I/O 操作。
     *
     * @return 返回用于网络通信的套接字对象
     */
    tcp::socket& GetSocket();

private:
    /**
     * @brief 检查连接是否超时
     *
     * 该函数通过 `net::steady_timer` 设置一个定时器，启动异步操作等待定时器到期。
     * 如果定时器到期且没有任何其他操作，连接将被关闭，防止长时间无响应。
     */
    void CheckDeadline();

    /**
     * @brief 解析 GET 请求参数
     *
     * 该函数从请求中提取 GET 请求的 URL 参数，并将其存储在 `_get_params` 容器中，
     * 该操作是基于 `http::request` 对象的解析功能来完成的。
     */
    void PreParseGetParam();

    /**
     * @brief 处理 HTTP 响应
     *
     * 构建并发送 HTTP 响应。通过 `http::response` 类设置响应状态码、响应头以及响应体内容，
     * 然后通过 `http::async_write` 异步写入响应数据。
     */
    void WriteResponse();

    /**
     * @brief 处理 HTTP 请求
     *
     * 根据接收到的 HTTP 请求的类型（GET 或 POST），调用相应的逻辑处理函数。
     * - 对于 GET 请求，调用 `PreParseGetParam` 解析 URL 参数，并通过 `LogicSystem::HandleGet` 处理业务逻辑。
     * - 对于 POST 请求，调用 `LogicSystem::HandlePost` 处理业务逻辑。
     * 根据处理结果，设置 `_response` 响应对象的状态码和内容，并调用 `WriteResponse` 发送响应。
     */
    void HandleReq();

    // 套接字对象，用于网络连接
    tcp::socket  _socket;

    // 读取缓冲区，保存读取的数据
    beast::flat_buffer  _buffer{ 8192 };

    // 请求消息对象，用于解析客户端发送的 HTTP 请求
    http::request<http::dynamic_body> _request;

    // 响应消息对象，用于发送 HTTP 响应
    http::response<http::dynamic_body> _response;

    // 定时器对象，用于检测连接超时
    net::steady_timer deadline_{
        _socket.get_executor(), std::chrono::seconds(60) // 设置超时时间为 60 秒
    };

    // GET 请求的 URL
    std::string _get_url;

    // 存储 GET 请求中的参数
    std::unordered_map<std::string, std::string> _get_params;
};