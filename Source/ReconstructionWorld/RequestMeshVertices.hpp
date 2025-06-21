#pragma once
#include"AbstractMessage.hpp"

// ����mesh�Ľڵ���Ϣ
class RequestMeshVertices : public AbstractMessage{
public:
	uint32_t requestVertexId;
	uint32_t idPackage;

	RequestMeshVertices(uint32_t requestVertexId, uint32_t idPackage) 
		: AbstractMessage("RequestMeshVertices") {
		this->requestVertexId = requestVertexId;
		this->idPackage = idPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ����mesh���ݰ�
		stream->writeData<uint32_t>(idPackage);
		stream->writeData<uint32_t>(requestVertexId);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};