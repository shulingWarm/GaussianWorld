#pragma once
#include"AbstractMessage.hpp"

class FaceFinishMessage : public AbstractMessage{
public:
	FaceFinishMessage() : AbstractMessage("FaceFinishMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����package id
		uint32_t idPackage = stream->readData<uint32_t>();
		// ���������Ҫȥ��mesh�Ž���������
		
	}
};