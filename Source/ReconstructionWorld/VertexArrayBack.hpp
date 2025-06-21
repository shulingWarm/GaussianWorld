#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"RequestMeshVertices.hpp"

class VertexArrayBack : public AbstractMessage{
public:
	VertexArrayBack() : AbstractMessage("VertexArrayBack") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 读取package id
		uint32_t idPackage = stream->readData<uint32_t>();
		// 读取起始id
		uint32_t vertexBeginId = stream->readData<uint32_t>();
		// 读取节点个数
		uint32_t vertexNum = stream->readData<uint32_t>();
		// 读取数据项
		float* vertexData = new float[vertexNum*3];
		// 记录到mesh package里面
		stream->receive((char*)vertexData, sizeof(float) * vertexNum * 3);
		// 从package里面获取 mesh
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// 把数据指针传入到mesh里面
		meshPackage->meshSolver->setVertexSequence(vertexBeginId, vertexNum, vertexData);
		delete[] vertexData;
		// 请求发送下一段数据
		RequestMeshVertices requestMessage(vertexBeginId + vertexNum, idPackage);
		messageManager->sendMessage(requestMessage);
	}
};