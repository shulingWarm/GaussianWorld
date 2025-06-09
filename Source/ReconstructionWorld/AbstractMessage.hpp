#pragma once
#include<iostream>
#include<string>
#include"StreamInterface.hpp"

class AbstractMessage;

// 消息管理器的接口
// 其实就是底层的消息实现
// 只不过这里要把它接口化
class MessageManagerInterface {
public:
	//发送消息
	virtual void sendMessage(AbstractMessage& message) = 0;

	//处理数据头
	virtual void receiveMessage(uint32_t messageHeader) = 0;
};

// 抽象的消息
class AbstractMessage {
public:
	//消息的名字
	std::string name;

	AbstractMessage(std::string name) {
		this->name = name;
	}

	//发送消息的逻辑
	virtual void send(StreamInterface* stream) = 0;

	//接收消息的逻辑
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};