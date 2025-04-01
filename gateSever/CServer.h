#pragma once

#include "const.h"

class CServer :public std::enable_shared_from_this<CServer>
{
public:
	//服务器监听对端的消息 需要一个 上下文 和 端口
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	//启动服务器
	void Start();
private:
	//接受对端的连接
	tcp::acceptor  _acceptor;
	//引用类型。没有拷贝和构造 
	net::io_context& _ioc;
	//接受对端连接的信息
	boost::asio::ip::tcp::socket   _socket;
};