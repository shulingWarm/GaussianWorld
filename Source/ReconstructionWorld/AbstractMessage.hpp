#pragma once
#include<iostream>
#include<string>
#include"StreamInterface.hpp"

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
	virtual void receive(StreamInterface* stream) = 0;
};