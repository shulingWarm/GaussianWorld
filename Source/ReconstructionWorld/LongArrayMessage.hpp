#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"
#include"LongArrayFinishFunctor.hpp"

class LongArrayMessage : public AbstractMessage {
public:
	char* dataArray = nullptr;
	uint32_t byteNum_ = 0;
	Ptr<LongArrayFinishFunctor> finishFunctor = nullptr;
	bool autoDelete = false;

	LongArrayMessage() : AbstractMessage("LongArrayMessage") {

	}

	LongArrayMessage(char* dataArray, uint32_t byteNum,
		Ptr<LongArrayFinishFunctor> finishFunctor, 
		bool autoDelete = false) : AbstractMessage("LongArrayMessage") {
		this->dataArray = dataArray;
		this->byteNum_ = byteNum;
		this->autoDelete = autoDelete;
		this->finishFunctor = finishFunctor;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// �ڱ��ؽ������ݰ�
		LongArrayPackage* longArrayPackage = new LongArrayPackage(
			this->dataArray, this->byteNum_, this->finishFunctor,this->autoDelete
		);
		// ע��long array�����ݰ�
		uint32_t packageId = 
			stream->getPackageManager()->registerPackageTask(longArrayPackage);
		// �������ݰ�id
		stream->writeData<uint32_t>(packageId);
		stream->writeData<uint32_t>(this->byteNum_);
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