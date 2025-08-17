#pragma once
#include"AbstractMessage.hpp"
#include"MeshGenPackage.hpp"
#include"MeshFinishFunctor.hpp"

class HunyuanMeshGenMessage : public AbstractMessage {
public:
	uint32_t idPackage;
	MeshFinishFunctor* finishFunctor;

	HunyuanMeshGenMessage(uint32_t idPackage,
		MeshFinishFunctor* finishFunctor) : AbstractMessage("HunyuanMeshGenMessage") {
		this->idPackage = idPackage;
		this->finishFunctor = finishFunctor;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 在这里新建一个mesh生成任务的package
		auto meshGenPackage = new MeshGenPackage(this->idPackage, this->finishFunctor);
		// 在本地注册这个mesh package
		uint32_t genTaskId = 
			stream->getPackageManager()->registerPackageTask(meshGenPackage);
		// 发送目标图片的id
		stream->writeData<uint32_t>(this->idPackage);
		// 发送mesh gen的id
		stream->writeData<uint32_t>(genTaskId);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};