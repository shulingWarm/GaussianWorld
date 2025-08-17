#pragma once
#include"AbstractMessage.hpp"
#include"MeshPackage.hpp"
#include"LongArrayPackage.hpp"
#include"MeshGenPackage.hpp"

class TextureFinishMessage : public AbstractMessage {
public:
	TextureFinishMessage() : AbstractMessage("TextureFinishMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收array package
		uint32_t idMeshPackage = stream->readData<uint32_t>();
		uint32_t idArrayPackage = stream->readData<uint32_t>();
		// 接收texture的channel个数
		uint32_t channel = stream->readData<uint32>();
		// 取出mesh
		auto meshPackage = (MeshPackage*)stream->getPackageManager()->getRemotePackage(idMeshPackage);
		auto mesh = meshPackage->meshSolver;
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idArrayPackage);
		// texture的数据
		auto textureData = (uint8_t*)arrayPackage->dataArray;
		// 像素数
		uint32_t pixelNum = mesh->getTextureSize(0) * mesh->getTextureSize(1);
		// 通过mesh设置texture数据
		mesh->setTexturePixelSequence(0, pixelNum, channel, textureData);
		// 找到mesh package里面对应的task package
		uint32_t taskPackageId = meshPackage->meshTaskId;
		// 取出task package
		MeshGenPackage* meshGenPackage = (MeshGenPackage*)stream->
			getPackageManager()->getLocalInfo(taskPackageId);
		// 判断是否存在回调函数
		if (meshGenPackage->meshFinishFunctor != nullptr) {
			meshGenPackage->meshFinishFunctor->meshFinishProcess(mesh, idMeshPackage);
		}
	}
};