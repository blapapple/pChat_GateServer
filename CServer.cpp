#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) :_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {
}

/*
 * 服务器的构造函数
 * 使用tcp::acceptor来监听传入的连接。
 */
void CServer::Start() {
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_connection = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_connection->GetSocket(), [self, new_connection](beast::error_code ec) {
		try {
			//出错放弃这连接，继续监听其他连接
			if (ec) {
				self->Start();
				return;
			}
			//创建新连接
			new_connection->Start();

			//继续监听，这里很重要，因为在上面的HttpConnection中会关闭连接，需要后续继续监听，否则后续会拒绝访问
			self->Start();
		}
		catch (std::exception& exp) {

		}
		});
}