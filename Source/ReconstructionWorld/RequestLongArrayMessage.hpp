#pragma once
#include"AbstractMessage.hpp"

class RequestLongArrayMessage : public AbstractMessage {
public:
	uint32_t idPackage;
	uint32_t idData;

	RequestLongArrayMessage(uint32_t idPackage, uint32_t idData) : 
		AbstractMessage("RequestLongArrayMessage") {
		this->idPackage = idPackage;
		this->idData = idData;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 写入package id
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idData);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};