#pragma once
#include<string>
#include<functional>
#include"GaussianSplatSolver.hpp"
#include"ImageList.hpp"
#include"MessageManager.hpp"
#include"ReconstructionPackage.hpp"
#include"ReconstructionMessage.hpp"

// �յ��ؽ����ʱ�Ļص�
class ReconstructionFinishFunctor {
public:
	virtual void onReconstructionFinish(GaussianSplatSolver* gaussianSplatPtr) = 0;
};

// lambda���ʽ��ʽ���ؽ�������ʱ�ص�
class ReconstructionFinishLambda : public ReconstructionFinishFunctor{
public:
	// lambda��ʽ�Ļص�
	typedef std::function<void(GaussianSplatSolver*)> FunctorType;
	FunctorType functor;

	ReconstructionFinishLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void onReconstructionFinish(GaussianSplatSolver* gaussianSplatPtr) override {
		this->functor(gaussianSplatPtr);
	}
};

// ִ���ؽ�ʱ��Ҫ��ͨ�Ų���
void requestReconstruction(std::string imageFolder, 
	ReconstructionFinishFunctor* reconstructionFinishFunctor,
	MessageManager* messageManager,
	StreamInterface* stream
) {
	// ��image folcer�½�ͼƬ��list
	// �������ؽ�package��������ʱ���ͷ�
	ImageList* imageList = new ImageList(imageFolder);
	// ׼���ؽ���package
	// �������ؽ����������ɵ���Ϣ���汻����
	ReconstructionPackage* reconPackage = new ReconstructionPackage(imageList);
	// ע�����package
	uint32_t idPackage = stream->getPackageManager()->registerPackageTask(reconPackage);
	// �����ؽ���message
	ReconstructionMessage reconMessage(idPackage);
	// ����Ϣ���ͳ�ȥ
	messageManager->sendMessage(reconMessage);
}