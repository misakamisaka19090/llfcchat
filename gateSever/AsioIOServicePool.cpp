#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;


AsioIOServicePool::AsioIOServicePool(std::size_t size)
	: _ioServices(size), _workGuards(size), _nextIOService(0)
{
	// 为每个 io_context 创建一个 work guard，防止其 run() 自动返回
	for (std::size_t i = 0; i < size; ++i) {
		_workGuards[i] = std::make_unique<WorkGuard>(_ioServices[i].get_executor());
	}

	// 为每个 io_context 启动一个独立线程，运行其事件循环
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}
AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

// 均匀分配任务到多个 io_context，避免单线程瓶颈。
boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
void AsioIOServicePool::Stop() {
	// 调用 stop() 使所有 io_context 停止运行（退出 run()）
	for (auto& ioService : _ioServices) {
		ioService.stop();
	}

	// 释放 work guard，允许 io_context 自行退出
	for (auto& work : _workGuards) {
		work.reset();
	}

	// 等待线程池中的所有线程退出
	for (auto& t : _threads) {
		if (t.joinable()) {
			t.join();
		}
	}
}
