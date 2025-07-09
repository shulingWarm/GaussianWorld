#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestTextureMessage.hpp"

class FaceUvFinishMessage : public AbstractMessage {
public:
	FaceUvFinishMessage() : AbstractMessage("FaceUvFinishMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {
		
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ����array package
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// ��ȡmesh package��array package
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		auto mesh = meshPackage->meshSolver;
		auto faceUvData = (int*)arrayPackage->dataArray;
		// ����face�ĸ���
		uint32_t faceNum = arrayPackage->byteNum / sizeof(int) / 3;
		// ��Ҫ��֤face�ĸ����͵���mesh��ʼ����face������ͬ
		if (faceNum != mesh->getFaceNum()) {
			throw std::runtime_error("Invalid face num");
		}
		// ��face uv��¼��mesh����
		mesh->setFaceUvSequence(0, faceNum, faceUvData);
		// ���array message
		stream->getPackageManager()->deleteRemotePackage(idArrayPackage);
		// ������������
		RequestTextureMessage requestMessage(idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};