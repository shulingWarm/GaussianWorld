#pragma once
#include"AbstractMessage.hpp"

// ͼƬ����������Ϣ
class ImageRowData : public AbstractMessage {
public:
	// ���ڷ��͵�ͼƬ����
	uint8_t* imgData;
	uint32_t imgWidth;
	//���η��͵���id
	uint32_t idRow;
	// ��Ϣ�����İ�id
	uint32_t idPackage;

	ImageRowData(uint8_t* imgData,
		uint32_t imgWidth,
		uint32_t idRow,
		uint32_t idPackage //��Ϣ�����İ�id
	) : AbstractMessage("ImageRowData") {
		this->imgData = imgData;
		this->imgWidth = imgWidth;
		this->idRow = idRow;
		this->idPackage = idPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// ������Ϣ��
		stream->writeData<uint32_t>(idPackage);
		// д��������
		stream->writeData<uint32_t>(idRow);
		// ����ͼƬ����
		stream->send((const char*)this->imgData, sizeof(uint8_t) * imgWidth * 4);
	}

	//������Ϣ���߼�
	//C++�����ʱ��ʵ�������н��յ����
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};