#pragma once
#include"PackageMsgManager.hpp"
#include"Types.hpp"
#include"ImageList.hpp"
#include<functional>
#include<memory>

// ������ÿ��ͼƬ��Ϣʱ�Ļص�
class SentPerImgFuncotr {
public:
	virtual void onPerImgSent(uint32_t idImage,
		uint32_t imagePackage) = 0;
};

// ����ÿ��ͼƬ�Ļص���lambda�汾
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

// ��ȡ���ImageList��ImageSequence������
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