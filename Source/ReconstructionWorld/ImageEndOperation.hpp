#pragma once
#include"ImageSolver.hpp"
#include<functional>
#include<iostream>
#include"Types.hpp"
#include"ImageList.hpp"

// 发送完图片后执行的操作
class ImageEndOperation {
public:
	//图片发送完成后执行的动作
	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) = 0;
};

// 用函数定义的operation
class ImageFuncEndOperation : public ImageEndOperation {
public:
	// 函数的数据类型
	using FuncType = std::function<void(Ptr<ImageSolver>, uint32_t)>;
	FuncType func;

	ImageFuncEndOperation(FuncType function) : func(function) {
		
	}

	virtual void imageEndOperation(Ptr<ImageSolver> image, uint32_t idPackage) override {
		this->func(image, idPackage);
	}
};

// 发送完所有图片的回调
// 这是发送图片序列的后处理回调
class ImgListEndFunctor {
public:
	virtual void imgEndOp(Ptr<ImageList> imgList) = 0;
};

// 发送完所有图片的lambda函数
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