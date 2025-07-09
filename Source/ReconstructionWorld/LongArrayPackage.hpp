#pragma once
#include"PackageMsgManager.hpp"

class LongArrayPackage : public PackageInfo {
public:
	// 完整的数据流
	char* dataArray;
	uint32_t byteNum;

	LongArrayPackage(uint32_t byteNum) {
		this->byteNum = byteNum;
		dataArray = new char[byteNum];
	}

	// 从指定起始位置的地方获取数据
	char* getDataFrom(uint32_t idData) {
		return dataArray + idData;
	}

	virtual ~LongArrayPackage() {
		delete[] dataArray;
	}
};