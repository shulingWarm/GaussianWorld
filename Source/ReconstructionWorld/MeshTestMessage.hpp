#pragma once
#include"AbstractMessage.hpp"

class MeshTestMessage : public AbstractMessage {
public:
	MeshTestMessage() : AbstractMessage("MeshTestMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 这只是发起一个请求，不需要带有任何信息
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};