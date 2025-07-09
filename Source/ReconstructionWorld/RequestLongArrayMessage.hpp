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

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// д��package id
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idData);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};