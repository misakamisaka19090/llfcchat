#pragma once

#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(tcp::socket socket);
	void Start();

private:
	//检测超时
	void CheckDeadline();

	//get请求的参数解析
	void PreParseGetParam();
	//应答函数
	void WriteResponse();
	//请求头
	void HandleReq();
	tcp::socket  _socket;
	// The buffer for performing reads.
	beast::flat_buffer  _buffer{ 8192 };

	// The request message.
	http::request<http::dynamic_body> _request;

	// The response message.
	http::response<http::dynamic_body> _response;

	// The timer for putting a deadline on connection processing. 
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60) 
	};

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

