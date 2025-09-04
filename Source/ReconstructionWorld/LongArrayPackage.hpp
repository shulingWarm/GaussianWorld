#pragma once
#include"PackageMsgManager.hpp"
#include"LongArrayFinishFunctor.hpp"

class LongArrayPackage : public PackageInfo {
public:
	// 完整的数据流
	char* dataArray;
	uint32_t byteNum;
	Ptr<LongArrayFinishFunctor> finishFunctor = nullptr;
	// 发送时每次发送的长度
	uint32_t sendLengthPerRequest = 64 << 10;

	LongArrayPackage(uint32_t byteNum) {
		this->byteNum = byteNum;
		dataArray = new char[byteNum];
	}

	LongArrayPackage(char* dataArray, uint32_t byteNum,
		Ptr<LongArrayFinishFunctor> finishFunctor) {
		this->dataArray = dataArray;
		this->byteNum = byteNum;
		this->finishFunctor = finishFunctor;
	}

	// 从指定起始位置的地方获取数据
	char* getDataFrom(uint32_t idData) {
		return dataArray + idData;
	}

	virtual ~LongArrayPackage() {
		delete[] dataArray;
	}
};