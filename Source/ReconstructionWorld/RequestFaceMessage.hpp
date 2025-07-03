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

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ���������package
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idFace);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};