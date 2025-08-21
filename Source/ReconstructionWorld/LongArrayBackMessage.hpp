#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"

class LongArrayBackMessage : public AbstractMessage {
public:
	uint32_t idPackage_ = 0;
	uint32_t requestId_ = 0;

	LongArrayBackMessage(uint32_t idPackage,
		uint32_t requestId) : AbstractMessage("LongArrayBackMessage") {
		this->idPackage_ = idPackage;
		this->requestId_ = requestId;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 读取package
		LongArrayPackage* package =
			(LongArrayPackage*)stream->getPackageManager()->getLocalInfo(idPackage_);
		uint32_t sendLength = package->sendLengthPerRequest;
		// 用于发送的数据
		char* sendData = package->getDataFrom(requestId_);
		stream->writeData<uint32_t>(idPackage_);
		stream->writeData<uint32_t>(requestId_);
		stream->writeData<uint32_t>(sendLength);
		// 发送数据
		stream->send(sendData, sendLength);
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

	static void messageSend(MessageManagerInterface* messageManager, uint32_t idPackage,
		uint32_t idData) {
		// 构造一个back message的实体
		LongArrayBackMessage backMessage(idPackage, idData);
		// 发送消息
		messageManager->sendMessage(backMessage);
	}
};