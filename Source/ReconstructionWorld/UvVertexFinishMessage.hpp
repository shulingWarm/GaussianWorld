#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestFaceUvMessage.hpp"

class UvVertexFinishMessage : public AbstractMessage{
public:
	UvVertexFinishMessage() : AbstractMessage("UvVertexFinishMessage") {
		
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) {

	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// ��ȡmesh��package id
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// ��ȡarray��package id
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		// ��ȡmesh package
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		// ��array package�����ȡarray
		float* uvVertexData = (float*)arrayPackage->dataArray;
		auto mesh = meshPackage->meshSolver;
		// ����vertex�ĸ���
		uint32_t vertexNum = arrayPackage->byteNum / sizeof(float) / 2;
		// ��array packageд�뵽mesh����
		mesh->setVertexUvSequence(0, vertexNum, uvVertexData);
		// ���remote package
		stream->getPackageManager()->deleteRemotePackage(idArrayPackage);
		// ����face uv��������Ϣ
		RequestFaceUvMessage requestMessage(idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};