#pragma once
#include"AbstractMessage.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconSingleImgMsg.hpp"
#include"ImageListPkg.hpp"
#include"ImageCommLib.hpp"
#include"ReconBeginMsg.hpp"
#include"ReconSingleFileImg.hpp"

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
		auto sendPerImageCallback = [stream, messageManager, idPackage](uint32_t idImage, 
			uint32_t imageArrayPackage, uint32_t imgFmt) {
			// 文件形式的单个图片发送完成的消息
			ReconSingleFileImg fileImgMsg(imageArrayPackage, imgFmt, idPackage);
			messageManager->sendMessage(fileImgMsg);
		};
		// 所有的image发送完成时的回调
		auto imgListEndFunctor = makePtr<ImgListEndLambda>([idPackage, messageManager] (Ptr<ImageList> imgList) {
			// 发送所有消息已经完成的通知
			ReconBeginMsg reconBeginMsg(idPackage);
			messageManager->sendMessage(reconBeginMsg);
		});
		// 发送每个图片之后的回调
		auto perImgFunctor = makePtr<SentPerImgLambda>(sendPerImageCallback);
		// 构造发送图片时的消息
		sendImgListByFile(tempPackage->imageList, perImgFunctor, messageManager,
			pkgManager, imgListEndFunctor);
	}
};