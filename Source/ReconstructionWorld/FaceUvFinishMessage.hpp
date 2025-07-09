#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestTextureMessage.hpp"

class FaceUvFinishMessage : public AbstractMessage {
public:
	FaceUvFinishMessage() : AbstractMessage("FaceUvFinishMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收array package
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		// 获取mesh package和array package
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		auto mesh = meshPackage->meshSolver;
		auto faceUvData = (int*)arrayPackage->dataArray;
		// 计算face的个数
		uint32_t faceNum = arrayPackage->byteNum / sizeof(int) / 3;
		// 需要保证face的个数和当初mesh初始化的face个数相同
		if (faceNum != mesh->getFaceNum()) {
			throw std::runtime_error("Invalid face num");
		}
		// 将face uv记录到mesh里面
		mesh->setFaceUvSequence(0, faceNum, faceUvData);
		// 清除array message
		stream->getPackageManager()->deleteRemotePackage(idArrayPackage);
		// 请求纹理数据
		RequestTextureMessage requestMessage(idMeshPackage);
		messageManager->sendMessage(requestMessage);
	}
};