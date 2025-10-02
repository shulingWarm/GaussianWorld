#pragma once
#include"MeshGenSource.hpp"
#include<string>

class FilePathGenSource : public MeshGenSource {
public:
	std::string imgFilePath;

	FilePathGenSource(std::string imgFilePath) {
		this->imgFilePath = imgFilePath;
		this->imgPathAvailable = true;
	}

	virtual std::string getImgPath() {
		return imgFilePath;
	}
};