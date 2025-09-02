#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconSingleImgMsg.hpp"
#include"ImageListPkg.hpp"

class ReconRecvMsg : public AbstractMessage{
public:
	ReconRecvMsg() : AbstractMessage("ReconRecvMsg") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {
		// 接收消息包
		uint32_t idPackage = stream->readData<uint32_t>();
		// 获取对应的消息
		auto pkgManager = stream->getPackageManager();
		auto tempPackage = (ReconstructionPackage*)pkgManager->getLocalInfo(idPackage);
		// 构造发送完每个图片时的回调
		auto sendPerImageCallback = [stream, messageManager, tempPackage](uint32_t idImage, 
			uint32_t imagePackage, uint32_t parentImgListPackageId) {
			// 发送单个图片发送完成的消息
			ReconSingleImgMsg singleImgMsg(idImage, imagePackage);
			messageManager->sendMessage(singleImgMsg);
		};
		// 构造发送图片序列的消息包
		auto imgListPkg = new ImageListPkg(tempPackage->imageList, 
			new SentPerImgLambda(sendPerImageCallback), false
		);
		// 注册发送图片序列的消息包
		uint32_t imgListPkgId = pkgManager->registerPackageTask(imgListPkg);
		// 构造发送图片时的消息

	}
};