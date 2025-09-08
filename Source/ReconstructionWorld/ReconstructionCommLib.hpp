#pragma once
#include<string>
#include<functional>
#include"GaussianSplatSolver.hpp"
#include"ImageList.hpp"
#include"MessageManager.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconstructionMessage.hpp"

// 执行重建时需要的通信操作
void requestReconstruction(std::string imageFolder, 
	Ptr<ReconstructionFinishFunctor> reconstructionFinishFunctor,
	MessageManager* messageManager,
	StreamInterface* stream
) {
	// 用image folcer新建图片的list
	// 它会在重建package被析构的时候释放
	auto imageList = makePtr<ImageList>(imageFolder);
	// 准备重建的package
	// 它会在重建结果发送完成的消息里面被销毁
	ReconstructionPackage* reconPackage = new ReconstructionPackage(imageList,
		reconstructionFinishFunctor);
	// 注册这个package
	uint32_t idPackage = stream->getPackageManager()->registerPackageTask(reconPackage);
	// 生成重建的message
	ReconstructionMessage reconMessage(idPackage);
	// 把消息发送出去
	messageManager->sendMessage(reconMessage);
}