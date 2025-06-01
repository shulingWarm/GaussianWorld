#pragma once
#include"MessageManager.hpp"

// 用于监听消息的runner
class MessageRunner : public FRunnable {
public:
	MessageManager* manager;

	MessageRunner(MessageManager* manager) {
		this->manager = manager;
	}

	// 执行过程
	virtual uint32 Run() override
	{
		//死循环，不断检查是否有消息
		while (true) {
			uint32 tag;
			manager->streamInterface->receive((char*)&tag, sizeof(uint32));
			// 判断tag是否在范围内
			if (manager->isTagInRange(tag)) {
				// 执行tag的接收逻辑
				manager->receiveMessage(tag);
			}
		}
	}
};