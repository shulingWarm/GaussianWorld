#pragma once
#include"ImageEndMessage.hpp"
#include"LongArrayMessage.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"

// 发送图片时的LongArray完成的回调
class LongArrayEndForImage : public LongArrayFinishFunctor {
public:
	ImageSolver* image;
	MessageManager* messageManager;
	ImageEndOperation* imgEndOperation;

	LongArrayEndForImage(ImageSolver* image,
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
void sendImage(ImageSolver* image, StreamInterface* stream, 
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