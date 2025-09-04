#pragma once
#include"ImageSolver.hpp"
#include<functional>
#include<iostream>
#include"Types.hpp"
#include"ImageList.hpp"

// ������ͼƬ��ִ�еĲ���
class ImageEndOperation {
public:
	//ͼƬ������ɺ�ִ�еĶ���
	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) = 0;
};

// �ú��������operation
class ImageFuncEndOperation : public ImageEndOperation {
public:
	// ��������������
	using FuncType = std::function<void(Ptr<ImageSolver>, uint32_t)>;
	FuncType func;

	ImageFuncEndOperation(FuncType function) : func(function) {
		
	}

	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) override {
		this->func(image, idPackage);
	}
};

// ����������ͼƬ�Ļص�
// ���Ƿ���ͼƬ���еĺ���ص�
class ImgListEndFunctor {
public:
	virtual void imgEndOp(Ptr<ImageList> imgList) = 0;
};

// ����������ͼƬ��lambda����
class ImgListEndLambda : public ImgListEndFunctor {
public:
	using FunctorType = std::function<void(Ptr<ImageList>)>;

	FunctorType functor;

	ImgListEndLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void imgEndOp(Ptr<ImageList> imgList) override {
		functor(imgList);
	}
};