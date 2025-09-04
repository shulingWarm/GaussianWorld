#pragma once
#include"PackageMsgManager.hpp"
#include"LongArrayFinishFunctor.hpp"

class LongArrayPackage : public PackageInfo {
public:
	// ������������
	char* dataArray;
	uint32_t byteNum;
	Ptr<LongArrayFinishFunctor> finishFunctor = nullptr;
	// ����ʱÿ�η��͵ĳ���
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

	// ��ָ����ʼλ�õĵط���ȡ����
	char* getDataFrom(uint32_t idData) {
		return dataArray + idData;
	}

	virtual ~LongArrayPackage() {
		delete[] dataArray;
	}
};