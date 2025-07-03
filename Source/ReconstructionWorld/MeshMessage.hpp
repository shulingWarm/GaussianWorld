#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"UEMeshSolver.hpp"
#include"RequestMeshVertices.hpp"

class MeshMessage : public AbstractMessage{
public:
	MeshMessage() : AbstractMessage("MeshMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// ����mesh�����ݰ���
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// ��ȡ���mesh�������ڵ�ͼƬid
		uint32_t idImagePackage = stream->readData<uint32_t>();
		// ��ȡ�ڵ�ĸ���
		uint32_t verticeNum = stream->readData<uint32_t>();
		// ��ȡface�ĸ���
		uint32_t faceNum = stream->readData<uint32_t>();
		auto meshSolver = UEMeshSolver::create();
		meshSolver->setFaceNum(faceNum);
		meshSolver->setVertexNum(verticeNum);
		// �½�mesh��package
		auto meshPackage = new MeshPackage(meshSolver, idImagePackage);
		// ��packageע�ᵽ������������
		stream->getPackageManager()->registerRemotePackage(idMeshPackage, meshPackage);
		// �ش�����ڵ���Ϣ
		RequestMeshVertices requestMessage(0, idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};