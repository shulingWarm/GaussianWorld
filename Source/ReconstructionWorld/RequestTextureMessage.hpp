#pragma once
#include"AbstractMessage.hpp"

class RequestTextureMessage : public AbstractMessage {
public:
	uint32_t idMeshPackage;

	RequestTextureMessage(uint32_t idMeshPackage) : AbstractMessage("RequestTextureMessage") {
		this->idMeshPackage = idMeshPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		stream->writeData<uint32_t>(idMeshPackage);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		
	}
};