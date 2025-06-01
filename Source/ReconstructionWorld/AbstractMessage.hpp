#pragma once
#include<iostream>
#include<string>
#include"StreamInterface.hpp"

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
	virtual void receive(StreamInterface* stream) = 0;
};