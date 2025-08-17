#pragma once
#include"PackageMsgManager.hpp"
#include"MeshFinishFunctor.hpp"

// ����mesh����ʱ��package
class MeshGenPackage : public PackageInfo {
public:
	// ����mesh����ʱ��ͼƬpackage
	uint32_t idImagePackage;
	MeshFinishFunctor* meshFinishFunctor;

	MeshGenPackage(uint32_t idImagePackage, MeshFinishFunctor* finishFunctor) {
		this->idImagePackage = idImagePackage;
		this->meshFinishFunctor = finishFunctor;
	}
};