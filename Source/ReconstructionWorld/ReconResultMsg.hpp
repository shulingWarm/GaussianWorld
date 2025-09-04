#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"


class ReconResultMsg : public AbstractMessage {
public:
	ReconResultMsg() : AbstractMessage("ReconResultMsg") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收 long array的package
		uint32_t idPackage = stream->readData<uint32_t>();
		// 获取long array的package
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// 从数组里面解析3DGS的数据

	}
};