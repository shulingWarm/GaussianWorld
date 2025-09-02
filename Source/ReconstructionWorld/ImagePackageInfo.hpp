#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"
#include"Types.hpp"

//ͼƬ���͵İ���Ϣ
class ImagePackageInfo : public PackageInfo{
public:
	//ͼƬ��Ϣ�ĵ�ַ
	Ptr<ImageSolver> image;
	Ptr<ImageEndOperation> imageEndOperation;

	ImagePackageInfo(Ptr<ImageSolver> image, Ptr<ImageEndOperation> imageEndOperation) {
		this->image = image;
		this->imageEndOperation = imageEndOperation;
	}
};

// ֻ����ͼƬid��image package���������Ϊ�˷��������server��ָ��
// Ŀǰ��������Ҫ���κζ�����������ܻ�����ڴ�й©
class IdOnlyImagePackage : public PackageInfo {
public:
	IdOnlyImagePackage() {

	}
};