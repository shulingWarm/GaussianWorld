#pragma once
#include<iostream>
#include<string>
#include "PackageMsgManager.hpp"

// �������Ľӿ�
class StreamInterface {
public:
	virtual void receive(char* data, uint32_t dataSize)=0;
	virtual void send(const char* data, uint32_t dataSize) = 0;
	//��ȡ�ְ�������
	virtual PackageMsgManager* getPackageManager() = 0;

	//д������
	template<class T>
	void writeData(T value) {
		send((char*)&value, sizeof(T));
	}

	//��ȡ����
	template<class T>
	T readData() {
		T value;
		receive((char*)&value, sizeof(T));
		return value;
	}

	//д���ַ���
	void writeStr(std::string str) {
		//д���ַ�������
		writeData<uint32_t>(str.size());
		//д���ַ�������
		send((const char*)str.c_str(), str.size());
	}

	//��ȡ�ַ���
	std::string readStr() {
		//��ȡ�ַ�������
		uint32_t strLength = readData<uint32_t>();
		char buffer[1024];
		//����ַ������ȹ���Ļ�ֱ�ӱ���
		if (strLength >= 1024) {
			throw std::runtime_error("Too large string: " + std::to_string(strLength));
		}
		receive(buffer, strLength);
		buffer[strLength] = '\0';
		return std::string(buffer);
	}
};