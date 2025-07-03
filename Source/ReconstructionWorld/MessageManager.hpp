#pragma once
#include"StreamInterface.hpp"
#include"AbstractMessage.hpp"
#include"PackageMsgManager.hpp"
#include<vector>
#include<unordered_map>
#include"LogLibrary.h"

class MessageManager : public MessageManagerInterface {
public:
	//数据tag的活动范围
	static constexpr uint32_t TAG_RANGE = 20000;
	//数据流的处理接口
	StreamInterface* streamInterface = nullptr;
	//已经注册过的所有消息
	std::vector<AbstractMessage*> messageList;
	//事件名称和message id的对应
	std::unordered_map<std::string, uint32_t> nameIdMap;

	//判断一个tag是否在合理的范围内
	static bool isTagInRange(uint32_t messageHeader) {
		return messageHeader >= TAG_RANGE && messageHeader < TAG_RANGE + 1024;
	}

	MessageManager(StreamInterface* streamInterafce) {
		this->streamInterface = streamInterafce;
	}

	// 注册消息的处理接口
	void registerMessage(AbstractMessage* message) {
		//需要确认名字不能是空的
		if (message->name.empty()) {
			throw std::runtime_error("Empty message name!");
		}
		//名字需要确保不能是重复的
		if (nameIdMap.count(message->name)) {
			throw std::runtime_error("Repeat message name: " + message->name);
		}
		nameIdMap[message->name] = messageList.size();
		messageList.push_back(message);
	}

	//发送消息
	void sendMessage(AbstractMessage& message) override {
		//判断消息是否注册过
		if (nameIdMap.count(message.name) == 0) {
			throw std::runtime_error("Message name not found: " + message.name);
		}
		//写入数据头
		uint32_t messageHeader = nameIdMap.at(message.name) + TAG_RANGE;
		this->streamInterface->send((char*)&messageHeader, sizeof(uint32_t));
		//调用message本身的发送过程
		message.send(this->streamInterface);
	}

	//处理数据头
	void receiveMessage(uint32_t messageHeader) override {
		//消息队列需要恢复掉叠加的tag
		messageHeader -= TAG_RANGE;
		//检查header是否在合法范围内
		if (messageHeader >= this->messageList.size()) {
			throw std::runtime_error("Message header out of range.");
		}
		this->messageList[messageHeader]->receive(this->streamInterface, this);
	}
};