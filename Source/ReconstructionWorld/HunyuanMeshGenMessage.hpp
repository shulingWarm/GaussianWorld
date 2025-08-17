#pragma once
#include"AbstractMessage.hpp"
#include"MeshGenPackage.hpp"
#include"MeshFinishFunctor.hpp"

class HunyuanMeshGenMessage : public AbstractMessage {
public:
	uint32_t idPackage;
	MeshFinishFunctor* finishFunctor;

	HunyuanMeshGenMessage(uint32_t idPackage,
		MeshFinishFunctor* finishFunctor) : AbstractMessage("HunyuanMeshGenMessage") {
		this->idPackage = idPackage;
		this->finishFunctor = finishFunctor;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// �������½�һ��mesh���������package
		auto meshGenPackage = new MeshGenPackage(this->idPackage, this->finishFunctor);
		// �ڱ���ע�����mesh package
		uint32_t genTaskId = 
			stream->getPackageManager()->registerPackageTask(meshGenPackage);
		// ����Ŀ��ͼƬ��id
		stream->writeData<uint32_t>(this->idPackage);
		// ����mesh gen��id
		stream->writeData<uint32_t>(genTaskId);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};