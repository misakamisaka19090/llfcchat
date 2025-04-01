#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <memory>
#include <QStyle>
#include <QWidget>
#include <iostream>
#include <functional>
#include <QNetworkReply>
#include <QRegularExpression>

/**
 * @file global.h
 * @brief 全局变量、枚举和常量定义文件
 *
 * 该文件定义了一些全局变量、枚举类型和常量，用于不同模块之间共享的全局配置、状态和错误处理。
 */

/**
 * @brief 全局函数指针，用于刷新和重新应用Qt控件的样式。
 *
 * `repolish` 是一个接受 `QWidget*` 参数的全局函数指针，通常用于在动态修改样式表后重新应用控件的样式。
 */
extern std::function<void(QWidget*)> repolish;

/**
 * @enum ReqID
 * @brief HTTP请求的ID枚举
 *
 * 用于标识不同类型的HTTP请求，帮助区分发送的请求类型。
 */
enum ReqID {
    ID_GET_VARIFY_CODE = 1001, /**< 获取验证码的请求 */
    ID_REG_USER = 1002         /**< 注册用户的请求 */
};

/**
 * @enum ErrorCodes
 * @brief 错误代码枚举
 *
 * 用于标识操作中的错误类型。错误代码通常返回给上层调用者，以便进行错误处理。
 */
enum ErrorCodes {
    SUCCESS = 0,       /**< 操作成功 */
    ERR_JSON = 1,      /**< JSON解析失败 */
    ERR_NETWORK = 2    /**< 网络错误 */
};

/**
 * @enum Modules
 * @brief 模块标识枚举
 *
 * 定义了应用中的模块，例如注册模块等。此处暂时只有注册模块。
 */
enum Modules {
    REGISTERMOD = 0 /**< 注册模块 */
};

#endif // GLOBAL_H
