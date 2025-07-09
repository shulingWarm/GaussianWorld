#pragma once
#include"AbstractMessage.hpp"

class RequestTextureMessage : public AbstractMessage {
public:
	uint32_t idMeshPackage;

	RequestTextureMessage(uint32_t idMeshPackage) : AbstractMessage("RequestTextureMessage") {
		this->idMeshPackage = idMeshPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		stream->writeData<uint32_t>(idMeshPackage);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		
	}
};