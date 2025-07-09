#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestFaceUvMessage.hpp"

class UvVertexFinishMessage : public AbstractMessage{
public:
	UvVertexFinishMessage() : AbstractMessage("UvVertexFinishMessage") {
		
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 读取mesh的package id
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// 读取array的package id
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		// 获取mesh package
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		// 从array package里面获取array
		float* uvVertexData = (float*)arrayPackage->dataArray;
		auto mesh = meshPackage->meshSolver;
		// 计算vertex的个数
		uint32_t vertexNum = arrayPackage->byteNum / sizeof(float) / 2;
		// 将array package写入到mesh里面
		mesh->setVertexUvSequence(0, vertexNum, uvVertexData);
		// 清除remote package
		stream->getPackageManager()->deleteRemotePackage(idArrayPackage);
		// 发送face uv的请求信息
		RequestFaceUvMessage requestMessage(idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};