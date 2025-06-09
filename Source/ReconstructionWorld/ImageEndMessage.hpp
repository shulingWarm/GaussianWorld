#pragma once
#include"AbstractMessage.hpp"
#include<iostream>

// ��������ͼƬ����Ϣ
class ImageEndMessage : public AbstractMessage{
public:
	uint32_t idPackage;

	ImageEndMessage(uint32_t idPackage) : AbstractMessage("ImageEndMessage") {
		this->idPackage = idPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ������ɵ����ݰ�
		stream->writeData<uint32_t>(this->idPackage);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};