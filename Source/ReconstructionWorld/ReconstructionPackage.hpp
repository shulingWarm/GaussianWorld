#pragma once
#include"ImageList.hpp"
#include"PackageMsgManager.hpp"
#include"Types.hpp"

// �յ��ؽ����ʱ�Ļص�
class ReconstructionFinishFunctor {
public:
	virtual void onReconstructionFinish(Ptr<GaussianSplatSolver> gaussianSplatPtr) = 0;
};

// lambda���ʽ��ʽ���ؽ�������ʱ�ص�
class ReconstructionFinishLambda : public ReconstructionFinishFunctor {
public:
	// lambda��ʽ�Ļص�
	typedef std::function<void(Ptr<GaussianSplatSolver>)> FunctorType;
	FunctorType functor;

	ReconstructionFinishLambda(FunctorType functor) {
		this->functor = functor;
	}

	virtual void onReconstructionFinish(Ptr<GaussianSplatSolver> gaussianSplatPtr) override {
		this->functor(gaussianSplatPtr);
	}
};

// �����ؽ�����Ϣ��
class ReconstructionPackage : public PackageInfo {
public:
	// ��Ҫ���͵�ͼƬ�б�
	Ptr<ImageList> imageList;
	// ����ؽ�ʱ�Ļص�
	Ptr<ReconstructionFinishFunctor> finishFunctor;

	ReconstructionPackage(Ptr<ImageList> imageList,
		Ptr<ReconstructionFinishFunctor> finishFunctor
	) {
		this->imageList = imageList;
		this->finishFunctor = finishFunctor;
	}
};