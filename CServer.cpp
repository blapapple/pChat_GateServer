#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {
}

/*
 * 服务器的构造函数，初始化io_context和socket，并设置监听端口。
 * 使用tcp::acceptor来监听传入的连接。
 */
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
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

			//继续监听，这里很重要，因为在上面的HttpConnection中会关闭连接，需要后续继续监听，否则后续会拒绝访问
			self->Start();
		}
		catch (std::exception& exp) {

		}
		});
}