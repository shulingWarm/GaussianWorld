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
		uint32 tag;
		//��ѭ�������ϼ���Ƿ�����Ϣ
		while (true) {
			tag = manager->streamInterface->readData<uint32_t>();
			// �ж�tag�Ƿ��ڷ�Χ��
			if (manager->isTagInRange(tag)) {
				// ִ��tag�Ľ����߼�
				manager->receiveMessage(tag);
			}
		}
		return 0;
	}
};