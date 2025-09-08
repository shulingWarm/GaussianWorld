#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include"RequestLongArrayMessage.hpp"
#include"LongArrayFinishFunctor.hpp"

class LongArrayMessage : public AbstractMessage {
public:
	char* dataArray = nullptr;
	uint32_t byteNum_ = 0;
	Ptr<LongArrayFinishFunctor> finishFunctor = nullptr;
	bool autoDelete = false;

	LongArrayMessage() : AbstractMessage("LongArrayMessage") {

	}

	LongArrayMessage(char* dataArray, uint32_t byteNum,
		Ptr<LongArrayFinishFunctor> finishFunctor, 
		bool autoDelete = false) : AbstractMessage("LongArrayMessage") {
		this->dataArray = dataArray;
		this->byteNum_ = byteNum;
		this->autoDelete = autoDelete;
		this->finishFunctor = finishFunctor;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 在本地建立数据包
		LongArrayPackage* longArrayPackage = new LongArrayPackage(
			this->dataArray, this->byteNum_, this->finishFunctor,this->autoDelete
		);
		// 注册long array的数据包
		uint32_t packageId = 
			stream->getPackageManager()->registerPackageTask(longArrayPackage);
		// 发送数据包id
		stream->writeData<uint32_t>(packageId);
		stream->writeData<uint32_t>(this->byteNum_);
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