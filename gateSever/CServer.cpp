#include "CServer.h"
#include "HttpConnection.h"

//实现服务器的端口监听和连接管理，接受客户端连接并分配处理。


//构造函数，初始化 io_context 和监听器 _acceptor
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):
_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {
}

//启动异步监听，接受新连接并创建 HttpConnection 实例处理请求。
void CServer::Start()
{
	auto self = shared_from_this();
	//接收器 有异步结束的操作 把接收到的连接传给_socket
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		try {
			//出错则放弃这个连接，继续监听新链接
			if (ec) {
				self->Start();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			//继续监听
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
		});
}