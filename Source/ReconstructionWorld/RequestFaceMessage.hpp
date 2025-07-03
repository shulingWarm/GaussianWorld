#pragma once
#include"AbstractMessage.hpp"
#include<iostream>

class RequestFaceMessage : public AbstractMessage{
public:
	uint32_t idPackage;
	uint32_t idFace;

	RequestFaceMessage(uint32_t idPackage, 
		uint32_t idFace) : AbstractMessage("RequestFaceMessage") {
		this->idPackage = idPackage;
		this->idFace = idFace;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送请求的package
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idFace);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};