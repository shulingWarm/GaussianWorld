#pragma once
#include"ImageEndMessage.hpp"
#include"LongArrayMessage.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"
#include"ImageList.hpp"
#include"ImageListPkg.hpp"
#include"FormatLibrary.h"
#include"Types.hpp"

// 发送图片时的LongArray完成的回调
class LongArrayEndForImage : public LongArrayFinishFunctor {
public:
	Ptr<ImageSolver> image;
	MessageManager* messageManager;
	ImageEndOperation* imgEndOperation;

	LongArrayEndForImage(Ptr<ImageSolver> image,
		MessageManager* messageManager,
		ImageEndOperation* imgEndOperation
	) {
		this->image = image;
		this->messageManager = messageManager;
		this->imgEndOperation = imgEndOperation;
	}

	virtual void arrayFinishProcess(PackageMsgManager* packageManager,
		uint32_t idPackage) override {
		// 读取图片的宽高
		uint32_t imgWidth = image->getWidth();
		uint32_t imgHeight = image->getHeight();
		// 注册图片的信息
		// 这里是随便注册一个空的package就可以了
		uint32_t imagePackageId = packageManager->registerPackageTask(
			new IdOnlyImagePackage()
		);
		// 图片完成的消息
		ImageEndMessage imgEndMessage(idPackage, imgWidth, imgHeight, imagePackageId);
		messageManager->sendMessage(imgEndMessage);
		// 执行图片发送完成的消息
		imgEndOperation->imageEndOperation(image, imagePackageId);
	}
};

// 将图片发送到远端
void sendImage(Ptr<ImageSolver> image, StreamInterface* stream,
	MessageManager* messageManager, ImageEndOperation* imgEndOperation) {

	// LongArray发送完成时的回调
	auto arrayFinishFunctor = new LongArrayEndForImage(image,
		messageManager, imgEndOperation);

	// 从图片里面取出指针
	uint8_t* imgData = image->getRowData(0);
	// 获取完整的字节数
	uint32_t byteNum = image->getWidth() * image->getHeight() * 4;
	// 发送LongArray message
	LongArrayMessage arrayMessage((char*)imgData, byteNum, arrayFinishFunctor);
	messageManager->sendMessage(arrayMessage);
}

// 针对image的发送完成时的回调
class ImageListPerEndCallback : public ImageEndOperation {
public:
	Ptr<ImageList> imgList;
	Ptr<SentPerImgFuncotr> perImgFunctor;
	MessageManager* msgManager;
	// 目前已经发送完成的图片数
	uint32_t sentImgNum = 0;

	ImageListPerEndCallback(Ptr<ImageList> imgList, 
		Ptr<SentPerImgFuncotr> perImgFunctor,
		MessageManager* msgManager
	) {
		this->imgList = imgList;
		this->perImgFunctor = perImgFunctor;
		this->msgManager = msgManager;
		sentImgNum = 0;
	}

	// 发送下一个图片
	void sendNextImg() {
		// 下一个图片的id
		uint32_t nextId = this->sentImgNum;
		// 读取下一个image
		auto imgSolver = makePtr<ArrayImage>(FormatLibrary::convertToFString(
			this->imgList->imagePathList[nextId]
		));
		// 发送图片

	}

	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) override {

	}
};

// 发送图片的列表
void sendImgList(Ptr<ImageList> imgList, 
	Ptr<SentPerImgFuncotr> perImgFunctor,
	MessageManager* msgManager
) {
	// 初始化每个图片发送完成时的回调
	auto imgListPerEnd = makePtr<ImageListPerEndCallback>(imgList, 
		perImgFunctor, msgManager);
	// 调用img list发送下一个图片

}