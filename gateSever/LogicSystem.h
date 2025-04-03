#pragma once

#include "const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	//����·�����Ҳ����ö�Ӧ�Ĵ���������δ�ҵ��򷵻� false
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);

	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);

	//ע�� URL ·�����Ӧ�� GET ����������
	void RegGet(std::string, HttpHandler handler);

	void RegPost(std::string, HttpHandler handler);
private:
	LogicSystem();
	std::map<std::string, HttpHandler> _post_handlers;
	std::map<std::string, HttpHandler> _get_handlers;
};