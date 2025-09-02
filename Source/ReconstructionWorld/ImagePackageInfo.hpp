#pragma once
#include"PackageMsgManager.hpp"
#include"ImageSolver.hpp"
#include"ImageEndOperation.hpp"
#include"Types.hpp"

//图片类型的包信息
class ImagePackageInfo : public PackageInfo{
public:
	//图片信息的地址
	Ptr<ImageSolver> image;
	Ptr<ImageEndOperation> imageEndOperation;

	ImagePackageInfo(Ptr<ImageSolver> image, Ptr<ImageEndOperation> imageEndOperation) {
		this->image = image;
		this->imageEndOperation = imageEndOperation;
	}
};

// 只包含图片id的image package，这仅仅是为了方便后续给server发指令
// 目前来看不需要存任何东西，但这可能会存在内存泄漏
class IdOnlyImagePackage : public PackageInfo {
public:
	IdOnlyImagePackage() {

	}
};