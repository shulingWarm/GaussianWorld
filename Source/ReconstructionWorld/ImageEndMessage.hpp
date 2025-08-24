#pragma once
#include"AbstractMessage.hpp"
#include"ImagePackageInfo.hpp"
#include<iostream>
#include<functional>

// ��������ͼƬ����Ϣ
class ImageEndMessage : public AbstractMessage{
public:
	uint32_t idPackage;
	uint32_t imageWidth;
	uint32_t imageHeight;
	uint32_t imagePackageId;

	ImageEndMessage(uint32_t idPackage, // ����ָ����LongArrayPackage��id
		uint32_t imageWidth,
		uint32_t imageHeight,
		uint32_t imagePackageId
	) : AbstractMessage("ImageEndMessage") {
		this->idPackage = idPackage;
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->imagePackageId = imagePackageId;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ������ɵ����ݰ�
		// ֪ͨԶ�˰�ָ����package�����ͼƬ
		stream->writeData<uint32_t>(this->idPackage);
		// ����ͼƬ�Ŀ��
		stream->writeData<uint32_t>(this->imageWidth);
		stream->writeData<uint32_t>(this->imageHeight);
		// �����µ����ݰ�id
		stream->writeData<uint32_t>(this->imagePackageId);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};