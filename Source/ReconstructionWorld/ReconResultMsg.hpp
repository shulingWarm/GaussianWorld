#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"


class ReconResultMsg : public AbstractMessage {
public:
	ReconResultMsg() : AbstractMessage("ReconResultMsg") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ���� long array��package
		uint32_t idPackage = stream->readData<uint32_t>();
		// ��ȡlong array��package
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// �������������3DGS������

	}
};