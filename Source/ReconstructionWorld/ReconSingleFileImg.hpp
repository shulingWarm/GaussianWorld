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

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ����id array package
		stream->writeData<uint32_t>(idArrayPackage);
		stream->writeData<uint32_t>(imgFormat);
		stream->writeData<uint32_t>(this->reconPkgId);
		
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};