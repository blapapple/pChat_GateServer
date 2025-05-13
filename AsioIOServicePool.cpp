#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_workGuards(size), _nextIOService(0) {
	for (std::size_t i = 0; i < size; ++i) {
		_workGuards[i] = std::make_unique<WorkGuard>(boost::asio::make_work_guard(_ioServices[i]));
	}

	//�������ioservice����������̣߳�ÿ���߳��ڲ�����ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
	// �ͷ� work guards
	for (auto& guard : _workGuards) {
		guard->get_executor().context().stop();
		guard.reset();
	}

	// �ȴ������߳����
	for (auto& t : _threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	// �������
	_threads.clear();
	_workGuards.clear();
}