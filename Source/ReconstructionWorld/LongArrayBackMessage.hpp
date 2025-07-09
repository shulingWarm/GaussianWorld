#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayBackMessage : public AbstractMessage {
public:
	LongArrayBackMessage() : AbstractMessage("LongArrayBackMessage") {
		
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收package id
		uint32_t idPackage = stream->readData<uint32_t>();
		uint32_t idData = stream->readData<uint32_t>();
		// 读取数据的长度
		uint32_t dataLength = stream->readData<uint32_t>();
		// 获取package
		auto arrayPackage = (LongArrayPackage*)stream->getPackageManager()->getRemotePackage(idPackage);
		// 读取数据
		stream->receive(arrayPackage->getDataFrom(idData), dataLength);
		// 发送array的请求包
		RequestLongArrayMessage requestMessage(idPackage, idData + dataLength);
		messageManager->sendMessage(requestMessage);
	}
};