#pragma once
#include"AbstractMessage.hpp"

class RequestFaceUvMessage : public AbstractMessage
{
public:
	uint32_t idMeshPackage;

	RequestFaceUvMessage(uint32_t idMeshPackage) : AbstractMessage("RequestFaceUvMessage") {
		this->idMeshPackage = idMeshPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 发送请求的mesh package
		stream->writeData<uint32_t>(idMeshPackage);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};