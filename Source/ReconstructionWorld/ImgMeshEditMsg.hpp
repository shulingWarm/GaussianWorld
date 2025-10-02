#pragma once
#include"AbstractMessage.hpp"
#include"MeshFinishFunctor.hpp"
#include"MeshGenPackage.hpp"
#include"Types.hpp"

class ImgMeshEditMsg : public AbstractMessage {
public:
	uint32_t idImgPackage;
	std::string prompt;
	Ptr<MeshFinishFunctor> meshFinishFunctor;

	ImgMeshEditMsg(
		uint32_t idImgPackage, 
		std::string prompt, 
		Ptr<MeshFinishFunctor> meshFinishFunctor
	) : AbstractMessage("ImgMeshEditMsg") {
		this->idImgPackage = idImgPackage;
		this->prompt = prompt;
		this->meshFinishFunctor = meshFinishFunctor;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		// �½�mesh�����package
		auto meshGenPackage = new MeshGenPackage(idImgPackage, this->meshFinishFunctor);
		// �ڱ���ע�����package
		auto idTaskPackage = stream->getPackageManager()->registerPackageTask(
			meshGenPackage
		);
		// ����ͼƬ��id
		stream->writeData<uint32_t>(this->idImgPackage);
		// �����ؽ������package
		stream->writeData<uint32_t>(idTaskPackage);
		stream->writeStr(this->prompt);
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};