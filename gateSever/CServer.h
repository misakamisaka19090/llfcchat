#pragma once

#include "const.h"

class CServer :public std::enable_shared_from_this<CServer>
{
public:
	//�����������Զ˵���Ϣ ��Ҫһ�� ������ �� �˿�
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	//����������
	void Start();
private:
	//���ܶԶ˵�����
	tcp::acceptor  _acceptor;
	//�������͡�û�п����͹��� 
	net::io_context& _ioc;
	//���ܶԶ����ӵ���Ϣ
	boost::asio::ip::tcp::socket   _socket;
};