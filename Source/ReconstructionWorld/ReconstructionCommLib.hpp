#pragma once
#include<string>
#include<functional>
#include"GaussianSplatSolver.hpp"
#include"ImageList.hpp"
#include"MessageManager.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconstructionMessage.hpp"

// 收到重建结果时的回调
class ReconstructionFinishFunctor {
public:
	virtual void onReconstructionFinish(GaussianSplatSolver* gaussianSplatPtr) = 0;
};

// lambda表达式形式的重建结果完成时回调
class ReconstructionFinishLambda : public ReconstructionFinishFunctor{
public:
	// lambda形式的回调
	typedef std::function<void(GaussianSplatSolver*)> FunctorType;
	FunctorType functor;

	ReconstructionFinishLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void onReconstructionFinish(GaussianSplatSolver* gaussianSplatPtr) override {
		this->functor(gaussianSplatPtr);
	}
};

// 执行重建时需要的通信操作
void requestReconstruction(std::string imageFolder, 
	ReconstructionFinishFunctor* reconstructionFinishFunctor,
	MessageManager* messageManager,
	StreamInterface* stream
) {
	// 用image folcer新建图片的list
	// 它会在重建package被析构的时候释放
	ImageList* imageList = new ImageList(imageFolder);
	// 准备重建的package
	// 它会在重建结果发送完成的消息里面被销毁
	ReconstructionPackage* reconPackage = new ReconstructionPackage(imageList);
	// 注册这个package
	uint32_t idPackage = stream->getPackageManager()->registerPackageTask(reconPackage);
	// 生成重建的message
	ReconstructionMessage reconMessage(idPackage);
	// 把消息发送出去
	messageManager->sendMessage(reconMessage);
}