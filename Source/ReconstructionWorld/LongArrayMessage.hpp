#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayMessage : public AbstractMessage {
public:
	LongArrayMessage() : AbstractMessage("LongArrayMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {

	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收long array的package
		uint32_t idPackage = stream->readData<uint32_t>();
		// 接收数据长度
		uint32_t byteNum = stream->readData<uint32_t>();
		// 开辟long array message
		stream->getPackageManager()->registerRemotePackage(
			idPackage, new LongArrayPackage(byteNum)
		);
		// 请求下一组数据
		RequestLongArrayMessage requestMessage(
			idPackage, 0
		);
		messageManager->sendMessage(requestMessage);
	}
};