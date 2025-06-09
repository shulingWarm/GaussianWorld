#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"

//图片类型的包信息
class ImagePackageInfo : public PackageInfo{
public:
	//图片信息的地址
	ImageSolver* image;

	ImagePackageInfo(ImageSolver* image) {
		this->image = image;
	}

	virtual ~ImagePackageInfo() {
		delete image;
	}
};