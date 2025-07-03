#pragma once
#include"AbstractMessage.hpp"
#include"RequestFaceMessage.hpp"

class VertexFinishMessage : public AbstractMessage {
public:
	VertexFinishMessage() : AbstractMessage("VertexFinishMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ����id package
		uint32_t idPackage = stream->readData<uint32_t>();
		// ��������face����Ϣ
		RequestFaceMessage message(idPackage, 0);
		messageManager->sendMessage(message);
	}
};