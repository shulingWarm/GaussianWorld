#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"

//图片类型的包信息
class ImagePackageInfo : public PackageInfo{
public:
	//图片信息的地址
	ImageSolver* image;
	ImageEndOperation* imageEndOperation;

	ImagePackageInfo(ImageSolver* image, ImageEndOperation* imageEndOperation) {
		this->image = image;
		this->imageEndOperation = imageEndOperation;
	}

	virtual ~ImagePackageInfo() {
		delete image;
	}
};