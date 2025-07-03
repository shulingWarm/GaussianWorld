#pragma once
#include"AbstractMessage.hpp"
#include"MeshSolver.hpp"
#include"RequestFaceMessage.hpp"
#include"LogLibrary.h"

class FaceArrayBack : public AbstractMessage {
public:
	FaceArrayBack() : AbstractMessage("FaceArrayBack") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 读取package id
		uint32_t idPackage = stream->readData<uint32_t>();
		uint32_t idFace = stream->readData<uint32_t>();
		// 读取face的个数
		uint32_t faceNum = stream->readData<uint32_t>();
		// 读取face内容
		int* faceList = new int[faceNum * 3];
		stream->receive((char*)faceList, sizeof(int) * faceNum * 3);
		// 读取face内容
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		auto mesh = meshPackage->meshSolver;
		// 将face list写入mesh
		mesh->setFaceSequence(idFace, faceNum, faceList);
		// 添加完成之后删除face
		delete[] faceList;
		// 请求新的face
		RequestFaceMessage requestMessage(idPackage, idFace + faceNum);
		messageManager->sendMessage(requestMessage);
	}
};