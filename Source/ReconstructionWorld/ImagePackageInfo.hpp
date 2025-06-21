#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"

//ͼƬ���͵İ���Ϣ
class ImagePackageInfo : public PackageInfo{
public:
	//ͼƬ��Ϣ�ĵ�ַ
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