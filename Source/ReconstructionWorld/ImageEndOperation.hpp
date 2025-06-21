#pragma once
#include"ImageSolver.hpp"
#include<functional>
#include<iostream>

// ������ͼƬ��ִ�еĲ���
class ImageEndOperation {
public:
	//ͼƬ������ɺ�ִ�еĶ���
	virtual void imageEndOperation(ImageSolver* image, uint32_t idPackage) = 0;
};

// �ú��������operation
class ImageFuncEndOperation : public ImageEndOperation {
public:
	// ��������������
	using FuncType = std::function<void(ImageSolver*, uint32_t)>;
	FuncType func;

	ImageFuncEndOperation(FuncType function) : func(function) {
		
	}

	virtual void imageEndOperation(ImageSolver* image, uint32_t idPackage) override {
		this->func(image, idPackage);
	}
};