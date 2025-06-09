#pragma once
#include<iostream>
#include<string>
#include"StreamInterface.hpp"

class AbstractMessage;

// ��Ϣ�������Ľӿ�
// ��ʵ���ǵײ����Ϣʵ��
// ֻ��������Ҫ�����ӿڻ�
class MessageManagerInterface {
public:
	//������Ϣ
	virtual void sendMessage(AbstractMessage& message) = 0;

	//��������ͷ
	virtual void receiveMessage(uint32_t messageHeader) = 0;
};

// �������Ϣ
class AbstractMessage {
public:
	//��Ϣ������
	std::string name;

	AbstractMessage(std::string name) {
		this->name = name;
	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) = 0;

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream, MessageManagerInterface* messageManager) = 0;
};