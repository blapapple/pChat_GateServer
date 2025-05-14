#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <chrono>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <queue>
#include <mutex>
#include <cassert>

#include "Singleton.h"
#include "hiredis.h"


namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,				//Json��������
	RPCFailed = 1002,				//RPC����ʧ��
	VarifyExpired = 1003,			//��֤�����
	VarifyCodeErr = 1004,			//��֤�����
	UserExist = 1005,				//�û��Ѵ���
	PasswdErr = 1006,				//�������
	EmailNotMatch = 1007,			//���䲻ƥ��
	PasswdUpFailed = 1008,			//�������ʧ��
	PasswdInvalid = 1009,			//���벻�Ϸ�
};

#define CODEPREFIX "code_"

/**
* Defer�࣬���������������ǰ����ָ���ĺ���
*/
class Defer {
public:
	Defer(std::function<void()> func) : func_(func) {}
	~Defer() { func_(); };
private:
	std::function<void()> func_;
};
