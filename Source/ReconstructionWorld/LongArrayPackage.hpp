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
	bool autoDelete;

	LongArrayPackage(uint32_t byteNum) {
		this->byteNum = byteNum;
		dataArray = new char[byteNum];
	}

	LongArrayPackage(char* dataArray, uint32_t byteNum,
		Ptr<LongArrayFinishFunctor> finishFunctor, bool autoDelete=false) {
		this->dataArray = dataArray;
		this->byteNum = byteNum;
		this->finishFunctor = finishFunctor;
		this->autoDelete = autoDelete;
	}

	// ��ָ����ʼλ�õĵط���ȡ����
	char* getDataFrom(uint32_t idData) {
		return dataArray + idData;
	}

	virtual ~LongArrayPackage() {
		if(this->autoDelete)
			delete[] dataArray;
	}
};