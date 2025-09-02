#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"

// ����ִ���ؽ����̵���Ϣ��
class ReconstructionMessage : public AbstractMessage {
public:
	uint32_t idPackage = 0;

	// ���ﴫ������ؽ�package�İ�id
	ReconstructionMessage(uint32_t idPackage) : AbstractMessage("ReconstructionMessage") {
		this->idPackage = idPackage;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// ��������
		auto packageManager = stream->getPackageManager();
		// ��ȡ�ؽ���package
		auto reconPackage = (ReconstructionPackage*)packageManager->getLocalInfo(this->idPackage);
		// ��ȡimage list
		auto imageList = reconPackage->imageList;
		// ��ȡimage list�ĳ���
		uint32_t imageNum = imageList->getImageNum();
		// �����ؽ��İ�id
		stream->writeData<uint32_t>(this->idPackage);
		// ����ͼƬ�ĸ���
		stream->writeData<uint32_t>(imageNum);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {

	}
};