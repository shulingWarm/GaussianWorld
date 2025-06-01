#pragma once
#include"MessageManager.hpp"

// ���ڼ�����Ϣ��runner
class MessageRunner : public FRunnable {
public:
	MessageManager* manager;

	MessageRunner(MessageManager* manager) {
		this->manager = manager;
	}

	// ִ�й���
	virtual uint32 Run() override
	{
		//��ѭ�������ϼ���Ƿ�����Ϣ
		while (true) {
			uint32 tag;
			manager->streamInterface->receive((char*)&tag, sizeof(uint32));
			// �ж�tag�Ƿ��ڷ�Χ��
			if (manager->isTagInRange(tag)) {
				// ִ��tag�Ľ����߼�
				manager->receiveMessage(tag);
			}
		}
	}
};