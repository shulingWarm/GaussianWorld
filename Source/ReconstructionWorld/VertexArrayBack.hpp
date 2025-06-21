#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"RequestMeshVertices.hpp"

class VertexArrayBack : public AbstractMessage{
public:
	VertexArrayBack() : AbstractMessage("VertexArrayBack") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ��ȡpackage id
		uint32_t idPackage = stream->readData<uint32_t>();
		// ��ȡ��ʼid
		uint32_t vertexBeginId = stream->readData<uint32_t>();
		// ��ȡ�ڵ����
		uint32_t vertexNum = stream->readData<uint32_t>();
		// ��ȡ������
		float* vertexData = new float[vertexNum*3];
		// ��¼��mesh package����
		stream->receive((char*)vertexData, sizeof(float) * vertexNum * 3);
		// ��package�����ȡ mesh
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// ������ָ�봫�뵽mesh����
		meshPackage->meshSolver->setVertexSequence(vertexBeginId, vertexNum, vertexData);
		delete[] vertexData;
		// ��������һ������
		RequestMeshVertices requestMessage(vertexBeginId + vertexNum, idPackage);
		messageManager->sendMessage(requestMessage);
	}
};