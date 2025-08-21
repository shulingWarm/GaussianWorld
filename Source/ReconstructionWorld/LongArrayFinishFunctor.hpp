#pragma once
#include<iostream>
#include<functional>
#include"PackageMsgManager.hpp"

class LongArrayFinishFunctor {
public:
	// array发送完成时的回调
	virtual void arrayFinishProcess(PackageMsgManager* packageManager, 
		uint32_t idPackage) = 0;
};

class LongArrayLambdaFinishFunctor : public LongArrayFinishFunctor {
public:
	using FuncType = std::function<void(PackageMsgManager*, uint32_t)>;
	FuncType functor;

	LongArrayLambdaFinishFunctor(FuncType functor) {
		this->functor = functor;
	}

	virtual void arrayFinishProcess(PackageMsgManager* packageManager,
		uint32_t idPackage) override {
		this->functor(packageManager, idPackage);
	}
};