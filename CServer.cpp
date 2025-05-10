#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {
}

/*
 * �������Ĺ��캯������ʼ��io_context��socket�������ü����˿ڡ�
 * ʹ��tcp::acceptor��������������ӡ�
 */
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
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

			//�����������������Ҫ����Ϊ�������HttpConnection�л�ر����ӣ���Ҫ�����������������������ܾ�����
			self->Start();
		}
		catch (std::exception& exp) {

		}
		});
}