#pragma once
#include"AbstractMessage.hpp"
#include"MeshSolver.hpp"
#include"RequestFaceMessage.hpp"
#include"LogLibrary.h"

class FaceArrayBack : public AbstractMessage {
public:
	FaceArrayBack() : AbstractMessage("FaceArrayBack") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ��ȡpackage id
		uint32_t idPackage = stream->readData<uint32_t>();
		uint32_t idFace = stream->readData<uint32_t>();
		// ��ȡface�ĸ���
		uint32_t faceNum = stream->readData<uint32_t>();
		// ��ȡface����
		int* faceList = new int[faceNum * 3];
		stream->receive((char*)faceList, sizeof(int) * faceNum * 3);
		// ��ȡface����
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		auto mesh = meshPackage->meshSolver;
		// ��face listд��mesh
		mesh->setFaceSequence(idFace, faceNum, faceList);
		// ������֮��ɾ��face
		delete[] faceList;
		// �����µ�face
		RequestFaceMessage requestMessage(idPackage, idFace + faceNum);
		messageManager->sendMessage(requestMessage);
	}
};