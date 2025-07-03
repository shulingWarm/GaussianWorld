#pragma once
#include"AbstractMessage.hpp"

class FaceFinishMessage : public AbstractMessage{
public:
	FaceFinishMessage() : AbstractMessage("FaceFinishMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收package id
		uint32_t idPackage = stream->readData<uint32_t>();
		// 这后续就需要去把mesh放进场景里了
		
	}
};