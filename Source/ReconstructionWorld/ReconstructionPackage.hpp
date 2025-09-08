#pragma once
#include"ImageList.hpp"
#include"PackageMsgManager.hpp"
#include"Types.hpp"

// 收到重建结果时的回调
class ReconstructionFinishFunctor {
public:
	virtual void onReconstructionFinish(Ptr<GaussianSplatSolver> gaussianSplatPtr) = 0;
};

// lambda表达式形式的重建结果完成时回调
class ReconstructionFinishLambda : public ReconstructionFinishFunctor {
public:
	// lambda形式的回调
	typedef std::function<void(Ptr<GaussianSplatSolver>)> FunctorType;
	FunctorType functor;

	ReconstructionFinishLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void onReconstructionFinish(Ptr<GaussianSplatSolver> gaussianSplatPtr) override {
		this->functor(gaussianSplatPtr);
	}
};

// 用于重建的消息包
class ReconstructionPackage : public PackageInfo {
public:
	// 需要发送的图片列表
	Ptr<ImageList> imageList;
	// 完成重建时的回调
	Ptr<ReconstructionFinishFunctor> finishFunctor;

	ReconstructionPackage(Ptr<ImageList> imageList,
		Ptr<ReconstructionFinishFunctor> finishFunctor
	) {
		this->imageList = imageList;
		this->finishFunctor = finishFunctor;
	}
};