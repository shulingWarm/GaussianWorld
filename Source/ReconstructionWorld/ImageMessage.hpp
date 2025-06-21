#pragma once
#include "AbstractMessage.hpp"
#include "ImageSolver.hpp"
#include "ImagePackageInfo.hpp"
#include"ImageEndOperation.hpp"

// 这是图片消息的数据头
// 但这里面并不包含有效的像素数据信息
class ImageMessage : public AbstractMessage{
public:
	//消息中管理的信息
	ImageSolver* image;
	//图片发送完成后的消息
	ImageEndOperation* imageEndOperation;

	//工作的时候需要传入一个图片信息
	ImageMessage(ImageSolver* image,
		ImageEndOperation* imageEndOperation //图片发送完成后的operation
	) : AbstractMessage("ImageMessage") {
		this->image = image;
		// 记录图片发送完成后的消息
		this->imageEndOperation = imageEndOperation;
	}

	//发送消息的逻辑
	void send(StreamInterface* stream) override {
		// 注册分包数据
		uint32_t packageId = stream->getPackageManager()->registerPackageTask(
			new ImagePackageInfo(this->image, this->imageEndOperation)
		);
		//发送图片的包id
		stream->writeData<uint32_t>(packageId);
		//发送图片基本的宽高信息
		stream->writeData<uint32_t>(image->getWidth());
		stream->writeData<uint32_t>(image->getHeight());
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};