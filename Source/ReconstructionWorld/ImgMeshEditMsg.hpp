#pragma once
#include"AbstractMessage.hpp"
#include"MeshFinishFunctor.hpp"
#include"MeshGenPackage.hpp"
#include"Types.hpp"

class ImgMeshEditMsg : public AbstractMessage {
public:
	uint32_t idImgPackage;
	std::string prompt;
	Ptr<MeshFinishFunctor> meshFinishFunctor;

	ImgMeshEditMsg(
		uint32_t idImgPackage, 
		std::string prompt, 
		Ptr<MeshFinishFunctor> meshFinishFunctor
	) : AbstractMessage("ImgMeshEditMsg") {
		this->idImgPackage = idImgPackage;
		this->prompt = prompt;
		this->meshFinishFunctor = meshFinishFunctor;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 新建mesh任务的package
		auto meshGenPackage = new MeshGenPackage(idImgPackage, this->meshFinishFunctor);
		// 在本地注册这个package
		auto idTaskPackage = stream->getPackageManager()->registerPackageTask(
			meshGenPackage
		);
		// 发送图片的id
		stream->writeData<uint32_t>(this->idImgPackage);
		// 发送重建任务的package
		stream->writeData<uint32_t>(idTaskPackage);
		stream->writeStr(this->prompt);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};