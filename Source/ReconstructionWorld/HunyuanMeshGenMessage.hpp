#pragma once
#include"AbstractMessage.hpp"

class HunyuanMeshGenMessage : public AbstractMessage {
public:
	uint32_t idPackage;

	HunyuanMeshGenMessage(uint32_t idPackage) : AbstractMessage("HunyuanMeshGenMessage") {
		this->idPackage = idPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ����Ŀ��ͼƬ��id
		stream->writeData<uint32_t>(this->idPackage);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};