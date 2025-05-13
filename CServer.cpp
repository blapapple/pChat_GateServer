#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) :_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {
}

/*
 * �������Ĺ��캯��
 * ʹ��tcp::acceptor��������������ӡ�
 */
void CServer::Start() {
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_connection = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_connection->GetSocket(), [self, new_connection](beast::error_code ec) {
		try {
			//������������ӣ�����������������
			if (ec) {
				self->Start();
				return;
			}
			//����������
			new_connection->Start();

			//�����������������Ҫ����Ϊ�������HttpConnection�л�ر����ӣ���Ҫ�����������������������ܾ�����
			self->Start();
		}
		catch (std::exception& exp) {

		}
		});
}