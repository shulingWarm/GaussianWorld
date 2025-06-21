#pragma once
#include"AbstractMessage.hpp"

class HunyuanMeshGenMessage : public AbstractMessage {
public:
	uint32_t idPackage;

	HunyuanMeshGenMessage(uint32_t idPackage) : AbstractMessage("HunyuanMeshGenMessage") {
		this->idPackage = idPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送目标图片的id
		stream->writeData<uint32_t>(this->idPackage);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};