#pragma once
#include "Singleton.h"
#include <functional>
#include <map>
#include "const.h"


class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

/**
 * @brief 逻辑处理系统类
 *
 * 该类负责管理 HTTP 请求的路由注册和请求的处理。它支持注册 GET 和 POST 请求的路由，并根据请求路径调用对应的处理函数。
 * 通过 `RegGet()` 和 `RegPost()` 方法，将路由与处理函数关联，`HandleGet()` 和 `HandlePost()` 方法用于根据请求路径调用相应的处理函数。
 *
 * 主要功能实现：
 *
 * 函数：RegGet()/RegPost()
 * - 将 URL 路径和对应的处理函数保存到 `_get_handlers` 或 `_post_handlers` 中。
 * - 可以通过路径查找并调用相应的处理函数来处理 HTTP 请求。
 *
 * 函数：HandleGet()/HandlePost()
 * - 根据传入的请求路径，查找对应的 GET 或 POST 处理函数。
 * - 如果找到对应的处理函数，则调用该函数来处理请求；否则，返回 `false`。
 *
 * 数据成员：
 * - `_get_handlers`：存储 GET 请求路径和处理函数的映射表。
 * - `_post_handlers`：存储 POST 请求路径和处理函数的映射表。
 */
class LogicSystem : public Singleton<LogicSystem>
{
    friend class Singleton<LogicSystem>;

public:
    /**
     * @brief 根据路径查找并调用对应的 GET 请求处理函数。
     *
     * 该函数根据传入的路径查找是否存在对应的处理函数。如果存在，则调用该函数来处理请求。
     * 如果路径未注册，则返回 `false`。
     *
     * @param path 请求的路径。
     * @param con 共享的 HTTP 连接对象。
     * @return 如果路径有对应的处理函数，则返回 `true`，否则返回 `false`。
     */
    bool HandleGet(std::string path, std::shared_ptr<HttpConnection> con);

    /**
     * @brief 根据路径查找并调用对应的 POST 请求处理函数。
     *
     * 该函数根据传入的路径查找是否存在对应的处理函数。如果存在，则调用该函数来处理请求。
     * 如果路径未注册，则返回 `false`。
     *
     * @param path 请求的路径。
     * @param con 共享的 HTTP 连接对象。
     * @return 如果路径有对应的处理函数，则返回 `true`，否则返回 `false`。
     */
    bool HandlePost(std::string path, std::shared_ptr<HttpConnection> con);

    /**
     * @brief 注册 GET 请求的路由和处理函数。
     *
     * 该函数用于将 URL 路径和对应的 GET 请求处理函数关联起来。当收到该路径的 GET 请求时，将调用对应的处理函数。
     *
     * @param url 路径。
     * @param handler 处理该路径的函数。
     */
    void RegGet(std::string url, HttpHandler handler);

    /**
     * @brief 注册 POST 请求的路由和处理函数。
     *
     * 该函数用于将 URL 路径和对应的 POST 请求处理函数关联起来。当收到该路径的 POST 请求时，将调用对应的处理函数。
     *
     * @param url 路径。
     * @param handler 处理该路径的函数。
     */
    void RegPost(std::string url, HttpHandler handler);

private:
    // 构造函数，用于初始化路由
    LogicSystem();

    // 存储 GET 请求路径和对应的处理函数的映射表
    std::map<std::string, HttpHandler> _get_handlers;

    // 存储 POST 请求路径和对应的处理函数的映射表
    std::map<std::string, HttpHandler> _post_handlers;
};
