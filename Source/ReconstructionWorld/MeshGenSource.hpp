#pragma once
#include<string>
#include<iostream>

// 这是一个抽象类，用于表示当初Mesh的生成方式
class MeshGenSource {
public:
	bool imgPathAvailable = false;

	virtual std::string getImgPath() {
		throw std::runtime_error("getImgPath not implemented");
		return "";
	}
};