#pragma once
#include<string>
#include<iostream>

// ����һ�������࣬���ڱ�ʾ����Mesh�����ɷ�ʽ
class MeshGenSource {
public:
	bool imgPathAvailable = false;

	virtual std::string getImgPath() {
		throw std::runtime_error("getImgPath not implemented");
		return "";
	}
};