#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"

// 这是执行重建过程的消息包
class ReconstructionMessage : public AbstractMessage {
public:
	uint32_t idPackage = 0;

	// 这里传入的是重建package的包id
	ReconstructionMessage(uint32_t idPackage) : AbstractMessage("ReconstructionMessage") {
		this->idPackage = idPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 包管理器
		auto packageManager = stream->getPackageManager();
		// 获取重建的package
		auto reconPackage = (ReconstructionPackage*)packageManager->getLocalInfo(this->idPackage);
		// 获取image list
		auto imageList = reconPackage->imageList;
		// 获取image list的长度
		uint32_t imageNum = imageList->getImageNum();
		// 发送重建的包id
		stream->writeData<uint32_t>(this->idPackage);
		// 发送图片的个数
		stream->writeData<uint32_t>(imageNum);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};