#pragma once

// C++ 标准库
#include <map>
#include <queue>
#include <memory>
#include <atomic>
#include <string>
#include <cassert>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <condition_variable>

// Boost 库
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

// JSON 处理
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

// MySQL 数据库
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>

// Redis
#include "hiredis.h"

// 其他工具
#include "Singleton.h"

// 命名空间别名

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,    // JSON 解析错误
	RPCFailed = 1002,     // RPC 请求失败
	VarifyExpired = 1003, // 验证码过期
	VarifyCodeErr = 1004, // 验证码错误
	UserExist = 1005,     // 用户已存在
	PasswdErr = 1006,     // 密码错误
	EmailNotMatch = 1007, // 邮箱不匹配
	PasswdUpFailed = 1008, // 密码更新失败
	PasswdInvalid = 1009, // 密码无效
	TokenInvalid = 1010,  // Token 失效
	UidInvalid = 1011     // UID 无效
};


// Defer类
class Defer {
public:
	// 接受一个lambda表达式或者函数指针
	Defer(std::function<void()> func) : func_(func) {}

	// 析构函数中执行传入的函数
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX  "code_"


