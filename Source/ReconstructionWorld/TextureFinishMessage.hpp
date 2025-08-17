#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"MeshGenPackage.hpp"

class TextureFinishMessage : public AbstractMessage {
public:
	TextureFinishMessage() : AbstractMessage("TextureFinishMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����array package
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		// ����texture��channel����
		uint32_t channel = stream->readData<uint32>();
		// ȡ��mesh
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto mesh = meshPackage->meshSolver;
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		// texture������
		auto textureData = (uint8_t*)arrayPackage->dataArray;
		// ������
		uint32_t pixelNum = mesh->getTextureSize(0) * mesh->getTextureSize(1);
		// ͨ��mesh����texture����
		mesh->setTexturePixelSequence(0, pixelNum, channel, textureData);
		// �ҵ�mesh package�����Ӧ��task package
		uint32_t taskPackageId = meshPackage->meshTaskId;
		// ȡ��task package
		MeshGenPackage* meshGenPackage = (MeshGenPackage*)stream->
			getPackageManager()->getLocalInfo(taskPackageId);
		// �ж��Ƿ���ڻص�����
		if (meshGenPackage->meshFinishFunctor != nullptr) {
			meshGenPackage->meshFinishFunctor->meshFinishProcess(mesh, idMeshPackage);
		}
	}
};