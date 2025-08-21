#pragma once
#include"PackageMsgManager.hpp"
#include"MeshFinishFunctor.hpp"

// ����mesh����ʱ��package
class MeshGenPackage : public PackageInfo {
public:
	// ����mesh����ʱ��ͼƬpackage
	uint32_t idImagePackage;
	MeshFinishFunctor* meshFinishFunctor = nullptr;

	MeshGenPackage(uint32_t idImagePackage, MeshFinishFunctor* finishFunctor) {
		this->idImagePackage = idImagePackage;
		this->meshFinishFunctor = finishFunctor;
	}

	virtual ~MeshGenPackage() {
		if (meshFinishFunctor != nullptr) {
			delete meshFinishFunctor;
		}
	}
};