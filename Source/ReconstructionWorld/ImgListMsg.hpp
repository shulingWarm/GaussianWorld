#pragma once
#include"AbstractMessage.hpp"

// 用于发送图片序列的消息
// 这个或许是可以弃用了的
class ImgListMsg : public AbstractMessage {
public:
	uint32_t imgListPkgId;

	ImgListMsg(uint32_t imgListPkgId) : AbstractMessage("ImgListMsg") {
		this->imgListPkgId = imgListPkgId;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 发送ImgList的
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};