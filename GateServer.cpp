#include "CServer.h"
#include "ConfigMgr.h"
/*
 * 这是一个简单的服务器示例，使用线程池处理传入的连接。
 * 服务器使用Boost.Asio进行异步I/O操作，并使用线程池并发处理请求。
 */
int main()
{
	ConfigMgr gConfigMgr;
	std::string gate_port_str = gConfigMgr["GateServer"]["port"];
	try
	{
		unsigned short port = static_cast<unsigned short>(8080);
		net::io_context ioc{ 1 };
		boost::asio::signal_set signals{ ioc, SIGINT, SIGTERM };
		//异步等待信号，若是接收到SIGINT或SIGTERM信号，则停止io_context
		signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
			if (error) {
				return;
			}
			ioc.stop();
			});


		std::make_shared<CServer>(ioc, port)->Start();
		std::cout << "Server is running on port " << port << std::endl;
		ioc.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}