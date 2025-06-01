#pragma once
#include"AbstractMessage.hpp"

//���ڲ��Ե�Hello Message
class HelloMessage : public AbstractMessage{
public:
	HelloMessage() : AbstractMessage("HelloMessage") {

	}

	//������Ϣ���߼�
	virtual void send(StreamInterface* stream) override {
		//�����ַ���
		stream->writeStr("Hello Linux");
	}

	//������Ϣ���߼�
	virtual void receive(StreamInterface* stream) override {
		//��ȡ�յ����ַ���
		std::string receivedMessage = stream->readStr();
		std::cout << "�յ���Ϣ: " << receivedMessage << std::endl;
	}
};