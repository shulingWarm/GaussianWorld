#pragma once
#include"AbstractMessage.hpp"
#include"ImagePackageInfo.hpp"
#include<iostream>
#include<functional>

// 结束发送图片的消息
class ImageEndMessage : public AbstractMessage{
public:
	uint32_t idPackage;
	uint32_t imageWidth;
	uint32_t imageHeight;
	uint32_t imagePackageId;

	ImageEndMessage(uint32_t idPackage, // 这里指的是LongArrayPackage的id
		uint32_t imageWidth,
		uint32_t imageHeight,
		uint32_t imagePackageId
	) : AbstractMessage("ImageEndMessage") {
		this->idPackage = idPackage;
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->imagePackageId = imagePackageId;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送完成的数据包
		// 通知远端把指定的package打包成图片
		stream->writeData<uint32_t>(this->idPackage);
		// 发送图片的宽高
		stream->writeData<uint32_t>(this->imageWidth);
		stream->writeData<uint32_t>(this->imageHeight);
		// 发送新的数据包id
		stream->writeData<uint32_t>(this->imagePackageId);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};