#pragma once
#include"AbstractMessage.hpp"
#include"RequestFaceMessage.hpp"

class VertexFinishMessage : public AbstractMessage {
public:
	VertexFinishMessage() : AbstractMessage("VertexFinishMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收id package
		uint32_t idPackage = stream->readData<uint32_t>();
		// 发送请求face的消息
		RequestFaceMessage message(idPackage, 0);
		messageManager->sendMessage(message);
	}
};