#include "CServer.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {

}

void CServer::Start() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		try {
			//出错放弃这连接，继续监听其他连接
			if (ec) {
				self->Start();
				return;
			}

			//创建新连接，并且创建HttpConnection类管理连接
			std::make_shared<
			Httpconnection

			//继续监听
		}
		catch (std::exception& exp) {

		}
		});
}