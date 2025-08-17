#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"UEMeshSolver.hpp"
#include"RequestMeshVertices.hpp"

class MeshMessage : public AbstractMessage{
public:
	MeshMessage() : AbstractMessage("MeshMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收mesh的数据包名
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// 接收当初的mesh任务package
		uint32_t meshTaskPackageId = stream->readData<uint32_t>();
		// 读取这个mesh当初属于的图片id
		uint32_t idImagePackage = stream->readData<uint32_t>();
		// 读取节点的个数
		uint32_t verticeNum = stream->readData<uint32_t>();
		// 读取face的个数
		uint32_t faceNum = stream->readData<uint32_t>();
		// texture的size
		uint32_t textureWidth = stream->readData<uint32_t>();
		uint32_t textureHeight = stream->readData<uint32_t>();
		auto meshSolver = UEMeshSolver::create();
		meshSolver->setFaceNum(faceNum);
		meshSolver->setVertexNum(verticeNum);
		// 设置texture的大小
		meshSolver->setTextureShape(textureWidth, textureHeight);
		// 新建mesh的package
		auto meshPackage = new MeshPackage(meshSolver, meshTaskPackageId);
		// 把package注册到包管理器里面
		stream->getPackageManager()->registerRemotePackage(idMeshPackage, meshPackage);
		// 回传请求节点信息
		RequestMeshVertices requestMessage(0, idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};