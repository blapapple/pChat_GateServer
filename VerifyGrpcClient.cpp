#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"

RPCConPool::RPCConPool(size_t poolSize, std::string host, std::string port)
	:poolSize_(poolSize), host_(host),
	port_(port), b_stop_(false) {
	for (size_t i = 0; i < poolSize_; ++i) {
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
		connections_.emplace(VarifyService::NewStub(channel));
	}
}

RPCConPool::~RPCConPool() {
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
	}
}

void RPCConPool::Close() {
	b_stop_ = true;
	condv_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPCConPool::GetConnection() {
	//��ȡ����ʱ�����ж����ӳ��Ƿ�رգ�����رգ��򷵻�nullptr
	std::unique_lock<std::mutex> lock(mutex_);
	condv_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
		});
	if (b_stop_) {
		return nullptr;
	}

	auto context = std::move(connections_.front());
	connections_.pop();
	return context;
}

void RPCConPool::ReturnConnection(std::unique_ptr<VarifyService::Stub> context) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}

	//�����ӷŻ����ӳ���
	connections_.emplace(std::move(context));
	condv_.notify_one();
}

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email) {
	ClientContext context;
	GetVarifyRsp reply;
	GetVarifyReq request;
	request.set_email(email);

	//�����ӳ��л�ȡ���Ӻ󣬽���Զ�̵��ã���ȡ��֤�룬�����óɹ��󣬷������ӵ����ӳ��С�reply�д洢����֤��
	auto stub = pool_->GetConnection();
	Status status = stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		pool_->ReturnConnection(std::move(stub));
		return reply;
	}
	else
	{
		reply.set_error(ErrorCodes::RPCFailed);
		pool_->ReturnConnection(std::move(stub));
		return reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient() {
	auto& gConfigMgr = ConfigMgr::Inst();
	std::string host = gConfigMgr["VarifyServer"]["Host"];
	std::string port = gConfigMgr["VarifyServer"]["Port"];
	pool_.reset(new RPCConPool(5, host, port));
}