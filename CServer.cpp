#include "CServer.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {

}

void CServer::Start() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		try {
			//������������ӣ�����������������
			if (ec) {
				self->Start();
				return;
			}

			//���������ӣ����Ҵ���HttpConnection���������
			std::make_shared<
			Httpconnection

			//��������
		}
		catch (std::exception& exp) {

		}
		});
}