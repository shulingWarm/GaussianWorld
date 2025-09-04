#pragma once
#include"PackageMsgManager.hpp"
#include"Types.hpp"
#include"ImageList.hpp"
#include<functional>
#include<memory>

// 发送完每个图片消息时的回调
class SentPerImgFuncotr {
public:
	virtual void onPerImgSent(uint32_t idImage,
		uint32_t imagePackage) = 0;
};

// 发送每个图片的回调的lambda版本
class SentPerImgLambda : public SentPerImgFuncotr  {
public:
	using FunctorType = std::function<void(uint32_t, uint32_t)>;
	FunctorType functor;

	SentPerImgLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void onPerImgSent(uint32_t idImage,
		uint32_t imagePackage) override {
		this->functor(idImage, imagePackage);
	}
};

// 获取这个ImageList叫ImageSequence更合适
class ImageListPkg : public PackageInfo{
public:
	bool autoDestory = false;
	ImageList* imageList;
	Ptr<SentPerImgFuncotr> sentPerImgFunctor;

	ImageListPkg(ImageList* imageList, Ptr<SentPerImgFuncotr> sentPerImgFunctor, 
		bool autoDestory=false) {
		this->imageList = imageList;
		this->autoDestory = autoDestory;
		this->sentPerImgFunctor = sentPerImgFunctor;
	}

	virtual ~ImageListPkg() {
		if (autoDestory) {
			delete this->imageList;
		}
	}
};