#pragma once
#include"AbstractMessage.hpp"

class ReconSingleFileImg : public AbstractMessage {
public:

	uint32_t idArrayPackage = 0;
	uint32_t imgFormat = 0;
	uint32_t reconPkgId;

	ReconSingleFileImg(uint32_t idArrayPackage, uint32_t imgFormat,
		uint32_t reconPkgId
	) : AbstractMessage("ReconSingleFileImg") {
		this->idArrayPackage = idArrayPackage;
		this->imgFormat = imgFormat;
		this->reconPkgId = reconPkgId;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送id array package
		stream->writeData<uint32_t>(idArrayPackage);
		stream->writeData<uint32_t>(imgFormat);
		stream->writeData<uint32_t>(this->reconPkgId);
		
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};