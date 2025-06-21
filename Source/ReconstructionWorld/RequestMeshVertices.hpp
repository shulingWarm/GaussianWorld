#pragma once
#include"AbstractMessage.hpp"

// 请求mesh的节点信息
class RequestMeshVertices : public AbstractMessage{
public:
	uint32_t requestVertexId;
	uint32_t idPackage;

	RequestMeshVertices(uint32_t requestVertexId, uint32_t idPackage) 
		: AbstractMessage("RequestMeshVertices") {
		this->requestVertexId = requestVertexId;
		this->idPackage = idPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送mesh数据包
		stream->writeData<uint32_t>(idPackage);
		stream->writeData<uint32_t>(requestVertexId);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};