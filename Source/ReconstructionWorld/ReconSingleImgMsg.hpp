#pragma once
#include"AbstractMessage.hpp"

class ReconSingleImgMsg : public AbstractMessage {
public:
	uint32_t idImage;
	uint32_t idPackage;

	ReconSingleImgMsg(uint32_t idImage, uint32_t idPackage) : AbstractMessage("ReconSingleImgMsg") {
		this->idImage = idImage;
		this->idPackage = idPackage;
	}


	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 发送reconstruction package
		stream->writeData<uint32_t>(this->idPackage);
		// 发送image id
		stream->writeData<uint32_t>(this->idImage);
		// 删除 image package
		stream->getPackageManager()->deletePackagInfo(this->idImage);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}

};