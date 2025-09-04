#pragma once
#include"AbstractMessage.hpp"

class ReconBeginMsg : public AbstractMessage {
public:
	uint32_t reconPkgId = 0;

	ReconBeginMsg(uint32_t reconPkgId) : AbstractMessage("ReconBeginMsg") {
		this->reconPkgId = reconPkgId;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送重建的package
		stream->writeData<uint32_t>(reconPkgId);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}


};