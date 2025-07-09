#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayBackMessage : public AbstractMessage {
public:
	LongArrayBackMessage() : AbstractMessage("LongArrayBackMessage") {
		
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		
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
};