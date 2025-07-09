#pragma once
#include"PackageMsgManager.hpp"

class LongArrayPackage : public PackageInfo {
public:
	// ������������
	char* dataArray;
	uint32_t byteNum;

	LongArrayPackage(uint32_t byteNum) {
		this->byteNum = byteNum;
		dataArray = new char[byteNum];
	}

	// ��ָ����ʼλ�õĵط���ȡ����
	char* getDataFrom(uint32_t idData) {
		return dataArray + idData;
	}

	virtual ~LongArrayPackage() {
		delete[] dataArray;
	}
};