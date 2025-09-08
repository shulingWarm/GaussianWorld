#pragma once
#include<string>
#include<functional>
#include"GaussianSplatSolver.hpp"
#include"ImageList.hpp"
#include"MessageManager.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconstructionMessage.hpp"

// ִ���ؽ�ʱ��Ҫ��ͨ�Ų���
void requestReconstruction(std::string imageFolder, 
	Ptr<ReconstructionFinishFunctor> reconstructionFinishFunctor,
	MessageManager* messageManager,
	StreamInterface* stream
) {
	// ��image folcer�½�ͼƬ��list
	// �������ؽ�package��������ʱ���ͷ�
	auto imageList = makePtr<ImageList>(imageFolder);
	// ׼���ؽ���package
	// �������ؽ����������ɵ���Ϣ���汻����
	ReconstructionPackage* reconPackage = new ReconstructionPackage(imageList,
		reconstructionFinishFunctor);
	// ע�����package
	uint32_t idPackage = stream->getPackageManager()->registerPackageTask(reconPackage);
	// �����ؽ���message
	ReconstructionMessage reconMessage(idPackage);
	// ����Ϣ���ͳ�ȥ
	messageManager->sendMessage(reconMessage);
}