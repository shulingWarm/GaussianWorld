#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayMessage : public AbstractMessage {
public:
	LongArrayMessage() : AbstractMessage("LongArrayMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����long array��package
		uint32_t idPackage = stream->readData<uint32_t>();
		// �������ݳ���
		uint32_t byteNum = stream->readData<uint32_t>();
		// ����long array message
		stream->getPackageManager()->registerRemotePackage(
			idPackage, new LongArrayPackage(byteNum)
		);
		// ������һ������
		RequestLongArrayMessage requestMessage(
			idPackage, 0
		);
		messageManager->sendMessage(requestMessage);
	}
};