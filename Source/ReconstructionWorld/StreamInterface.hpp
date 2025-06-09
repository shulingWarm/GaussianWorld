#pragma once
#include<iostream>
#include<string>
#include "PackageMsgManager.hpp"

// 数据流的接口
class StreamInterface {
public:
	virtual void receive(char* data, uint32_t dataSize)=0;
	virtual void send(const char* data, uint32_t dataSize) = 0;
	//获取分包管理器
	virtual PackageMsgManager* getPackageManager() = 0;

	//写入数据
	template<class T>
	void writeData(T value) {
		send((char*)&value, sizeof(T));
	}

	//读取数据
	template<class T>
	T readData() {
		T value;
		receive((char*)&value, sizeof(T));
		return value;
	}

	//写入字符串
	void writeStr(std::string str) {
		//写入字符串长度
		writeData<uint32_t>(str.size());
		//写入字符串内容
		send((const char*)str.c_str(), str.size());
	}

	//读取字符串
	std::string readStr() {
		//读取字符串长度
		uint32_t strLength = readData<uint32_t>();
		char buffer[1024];
		//如果字符串长度过大的话直接报错
		if (strLength >= 1024) {
			throw std::runtime_error("Too large string: " + std::to_string(strLength));
		}
		receive(buffer, strLength);
		buffer[strLength] = '\0';
		return std::string(buffer);
	}
};