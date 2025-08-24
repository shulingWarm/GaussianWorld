#pragma once
#include"AbstractMessage.hpp"
#include"LongArrayPackage.hpp"
#include<functional>

class RequestLongArrayMessage : public AbstractMessage {
public:
	uint32_t idPackage;
	uint32_t idData;

	// 发送array back message的functor
	// 因为不能循环引用，只能凑活着这么弄了
	using ArrayBackFunctor = std::function<void(MessageManagerInterface*,
		uint32_t,uint32_t)>;
	ArrayBackFunctor arrayBackFunctor;

	RequestLongArrayMessage(uint32_t idPackage, uint32_t idData) : 
		AbstractMessage("RequestLongArrayMessage") {
		this->idPackage = idPackage;
		this->idData = idData;
	}

	// 另一种构造函数，这是用来往message manager里面注册的
	RequestLongArrayMessage(ArrayBackFunctor arrayBackFunctor) : 
		AbstractMessage("RequestLongArrayMessage") {
		this->arrayBackFunctor = arrayBackFunctor;
		this->idPackage = 0;
		this->idData = 0;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) {
		// 写入package id
		stream->writeData<uint32_t>(this->idPackage);
		stream->writeData<uint32_t>(this->idData);
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) {
		// 接收package id
		uint32_t requestIdPackage = stream->readData<uint32_t>();
		// 请求的数据id
		uint32_t requestDataId = stream->readData<uint32_t>();
		// 获取package
		LongArrayPackage* package = (LongArrayPackage*)stream->getPackageManager()->getLocalInfo(requestIdPackage);
		// 判断如果发送完了所有数据，就执行发送完的后处理
		if (requestDataId >= package->byteNum) {
			// 调用结束时的回调
			if (package->finishFunctor != nullptr) {
				package->finishFunctor->arrayFinishProcess(stream->getPackageManager(),
					requestIdPackage);
			}
		}
		else {
			arrayBackFunctor(messageManager, requestIdPackage, requestDataId);
		}
	}
};