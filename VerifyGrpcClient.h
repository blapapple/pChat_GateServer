#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

/*
* RPC���ӳأ�ʹ��std::queue�洢����
 */
class RPCConPool {
public:
	RPCConPool(size_t poolSize, std::string host, std::string port);
	~RPCConPool();
	void Close();
	std::unique_ptr<VarifyService::Stub> GetConnection();
	void ReturnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::condition_variable condv_;
	std::mutex mutex_;
};

/*
* ��ȡ��֤��Ŀͻ��ˣ�ʹ��RPC���ӳأ�����Զ�̵���
*/
class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email);
private:
	VerifyGrpcClient();

	std::unique_ptr<RPCConPool> pool_;
};

