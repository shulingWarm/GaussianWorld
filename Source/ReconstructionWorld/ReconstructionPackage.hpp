#pragma once
#include"ImageList.hpp"
#include"PackageMsgManager.hpp"

// �����ؽ�����Ϣ��
class ReconstructionPackage : public PackageInfo {
public:
	// ��Ҫ���͵�ͼƬ�б�
	ImageList* imageList;

	ReconstructionPackage(ImageList* imageList) {
		this->imageList = imageList;
	}

	virtual ~ReconstructionPackage() {
		delete imageList;
	}
};