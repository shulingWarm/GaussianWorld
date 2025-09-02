#pragma once
#include"ImageList.hpp"
#include"PackageMsgManager.hpp"

// 用于重建的消息包
class ReconstructionPackage : public PackageInfo {
public:
	// 需要发送的图片列表
	ImageList* imageList;

	ReconstructionPackage(ImageList* imageList) {
		this->imageList = imageList;
	}

	virtual ~ReconstructionPackage() {
		delete imageList;
	}
};