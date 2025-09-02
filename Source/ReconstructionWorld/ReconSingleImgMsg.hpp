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


	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;

};