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
		uint32 tag;
		//死循环，不断检查是否有消息
		while (true) {
			tag = manager->streamInterface->readData<uint32_t>();
			// 判断tag是否在范围内
			if (manager->isTagInRange(tag)) {
				// 执行tag的接收逻辑
				manager->receiveMessage(tag);
			}
		}
		return 0;
	}
};