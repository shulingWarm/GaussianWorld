#pragma once
#include"AbstractMessage.hpp"

class RequestFaceUvMessage : public AbstractMessage
{
public:
	uint32_t idMeshPackage;

	RequestFaceUvMessage(uint32_t idMeshPackage) : AbstractMessage("RequestFaceUvMessage") {
		this->idMeshPackage = idMeshPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// ���������mesh package
		stream->writeData<uint32_t>(idMeshPackage);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};