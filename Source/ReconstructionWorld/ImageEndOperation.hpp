#pragma once
#include"ImageSolver.hpp"
#include<functional>
#include<iostream>

// 发送完图片后执行的操作
class ImageEndOperation {
public:
	//图片发送完成后执行的动作
	virtual void imageEndOperation(ImageSolver* image, uint32_t idPackage) = 0;
};

// 用函数定义的operation
class ImageFuncEndOperation : public ImageEndOperation {
public:
	// 函数的数据类型
	using FuncType = std::function<void(ImageSolver*, uint32_t)>;
	FuncType func;

	ImageFuncEndOperation(FuncType function) : func(function) {
		
	}

	virtual void imageEndOperation(ImageSolver* image, uint32_t idPackage) override {
		this->func(image, idPackage);
	}
};