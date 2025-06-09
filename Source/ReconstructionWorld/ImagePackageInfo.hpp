#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"

//ͼƬ���͵İ���Ϣ
class ImagePackageInfo : public PackageInfo{
public:
	//ͼƬ��Ϣ�ĵ�ַ
	ImageSolver* image;

	ImagePackageInfo(ImageSolver* image) {
		this->image = image;
	}

	virtual ~ImagePackageInfo() {
		delete image;
	}
};