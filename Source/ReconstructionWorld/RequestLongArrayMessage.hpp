#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include<functional>

class RequestLongArrayMessage : public AbstractMessage {
public:
	uint32_t idPackage;
	uint32_t idData;

	// ����array back message��functor
	// ��Ϊ����ѭ�����ã�ֻ�ܴջ�����ôŪ��
	using ArrayBackFunctor = std::function<void(MessageManagerInterface*,
		uint32_t,uint32_t)>;
	ArrayBackFunctor arrayBackFunctor;

	RequestLongArrayMessage(uint32_t idPackage, uint32_t idData) : 
		AbstractMessage("RequestLongArrayMessage") {
		this->idPackage = idPackage;
		this->idData = idData;
	}

	// ��һ�ֹ��캯��������������message manager����ע���
	RequestLongArrayMessage(ArrayBackFunctor arrayBackFunctor) : 
		AbstractMessage("RequestLongArrayMessage") {
		this->arrayBackFunctor = arrayBackFunctor;
		this->idPackage = 0;
		this->idData = 0;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		// д��package id
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idData);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����package id
		uint32_t requestIdPackage = stream->readData<uint32_t>();
		// ���������id
		uint32_t requestDataId = stream->readData<uint32_t>();
		// ��ȡpackage
		LongArrayPackage* package = (LongArrayPackage*)stream->getPackageManager()->getLocalInfo(requestIdPackage);
		// �ж�������������������ݣ���ִ�з�����ĺ���
		if (requestDataId >= package->byteNum) {
			// ���ý���ʱ�Ļص�
			if (package->finishFunctor != nullptr) {
				package->finishFunctor->arrayFinishProcess(stream->getPackageManager(),
					requestIdPackage);
			}
		}
		else {
			arrayBackFunctor(messageManager, requestIdPackage, requestDataId);
		}
	}
};