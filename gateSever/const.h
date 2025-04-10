#pragma once

#include <map>
#include <queue>
#include <memory>
#include <atomic>
#include <string>
#include <cassert>
#include <iostream>
#include <assert.h>
#include <functional>
#include <unordered_map>
#include <condition_variable>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "hiredis.h"
#include "Singleton.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,    // JSON 解析错误
	RPCFailed = 1002,     // RPC 请求错误
	VarifyExpired = 1003, // 验证码过期
	VarifyCodeErr = 1004, // 验证码错误
	UserExist = 1005,     // 用户已存在
	PasswdErr = 1006,     // 密码错误
	EmailNotMatch = 1007, // 邮箱不匹配
	PasswdUpFailed = 1008, // 密码更新失败
	PasswdInvalid = 1009  // 密码无效

};

#define CODEPREFIX "code_"