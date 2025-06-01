#pragma once
#include"AbstractMessage.hpp"

//用于测试的Hello Message
class HelloMessage : public AbstractMessage{
public:
	HelloMessage() : AbstractMessage("HelloMessage") {

	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) override {
		//发送字符串
		stream->writeStr("Hello Linux");
	}

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream) override {
		//读取收到的字符串
		std::string receivedMessage = stream->readStr();
		std::cout << "收到信息: " << receivedMessage << std::endl;
	}
};