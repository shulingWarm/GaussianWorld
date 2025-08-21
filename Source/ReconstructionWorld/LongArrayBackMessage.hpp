#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayBackMessage : public AbstractMessage {
public:
	uint32_t idPackage_ = 0;
	uint32_t requestId_ = 0;

	LongArrayBackMessage(uint32_t idPackage,
		uint32_t requestId) : AbstractMessage("LongArrayBackMessage") {
		this->idPackage_ = idPackage;
		this->requestId_ = requestId;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// ��ȡpackage
		LongArrayPackage* package =
			(LongArrayPackage*)stream->getPackageManager()->getLocalInfo(idPackage_);
		uint32_t sendLength = package->sendLengthPerRequest;
		// ���ڷ��͵�����
		char* sendData = package->getDataFrom(requestId_);
		stream->writeData<uint32_t>(idPackage_);
		stream->writeData<uint32_t>(requestId_);
		stream->writeData<uint32_t>(sendLength);
		// ��������
		stream->send(sendData, sendLength);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����package id
		uint32_t idPackage = stream->readData<uint32_t>();
		uint32_t idData = stream->readData<uint32_t>();
		// ��ȡ���ݵĳ���
		uint32_t dataLength = stream->readData<uint32_t>();
		// ��ȡpackage
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// ��ȡ����
		stream->receive(arrayPackage->getDataFrom(idData), dataLength);
		// ����array�������
		RequestLongArrayMessage requestMessage(idPackage, idData + dataLength);
		messageManager->sendMessage(requestMessage);
	}

	static void messageSend(MessageManagerInterface* messageManager, uint32_t idPackage,
		uint32_t idData) {
		// ����һ��back message��ʵ��
		LongArrayBackMessage backMessage(idPackage, idData);
		// ������Ϣ
		messageManager->sendMessage(backMessage);
	}
};