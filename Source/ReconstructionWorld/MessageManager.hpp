#pragma once
#include"StreamInterface.hpp"
#include"AbstractMessage.hpp"
#include"PackageMsgManager.hpp"
#include<vector>
#include<unordered_map>
#include"LogLibrary.h"

class MessageManager : public MessageManagerInterface {
public:
	//����tag�Ļ��Χ
	static constexpr uint32_t TAG_RANGE = 20000;
	//�������Ĵ���ӿ�
	StreamInterface* streamInterface = nullptr;
	//�Ѿ�ע�����������Ϣ
	std::vector<AbstractMessage*> messageList;
	//�¼����ƺ�message id�Ķ�Ӧ
	std::unordered_map<std::string, uint32_t> nameIdMap;

	//�ж�һ��tag�Ƿ��ں���ķ�Χ��
	static bool isTagInRange(uint32_t messageHeader) {
		return messageHeader >= TAG_RANGE && messageHeader < TAG_RANGE + 1024;
	}

	MessageManager(StreamInterface* streamInterafce) {
		this->streamInterface = streamInterafce;
	}

	// ע����Ϣ�Ĵ���ӿ�
	void registerMessage(AbstractMessage* message) {
		//��Ҫȷ�����ֲ����ǿյ�
		if (message->name.empty()) {
			throw std::runtime_error("Empty message name!");
		}
		//������Ҫȷ���������ظ���
		if (nameIdMap.count(message->name)) {
			throw std::runtime_error("Repeat message name: " + message->name);
		}
		nameIdMap[message->name] = messageList.size();
		messageList.push_back(message);
	}

	//������Ϣ
	void sendMessage(AbstractMessage& message) override {
		//�ж���Ϣ�Ƿ�ע���
		if (nameIdMap.count(message.name) == 0) {
			throw std::runtime_error("Message name not found: " + message.name);
		}
		//д������ͷ
		uint32_t messageHeader = nameIdMap.at(message.name) + TAG_RANGE;
		this->streamInterface->send((char*)&messageHeader, sizeof(uint32_t));
		//����message����ķ��͹���
		message.send(this->streamInterface);
	}

	//��������ͷ
	void receiveMessage(uint32_t messageHeader) override {
		//��Ϣ������Ҫ�ָ������ӵ�tag
		messageHeader -= TAG_RANGE;
		//���header�Ƿ��ںϷ���Χ��
		if (messageHeader >= this->messageList.size()) {
			throw std::runtime_error("Message header out of range.");
		}
		this->messageList[messageHeader]->receive(this->streamInterface, this);
	}
};