#pragma once
#include"AbstractMessage.hpp"
#include<iostream>

// 结束发送图片的消息
class ImageEndMessage : public AbstractMessage{
public:
	uint32_t idPackage;

	ImageEndMessage(uint32_t idPackage) : AbstractMessage("ImageEndMessage") {
		this->idPackage = idPackage;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		// 发送完成的数据包
		stream->writeData<uint32_t>(this->idPackage);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) override {

	}
};