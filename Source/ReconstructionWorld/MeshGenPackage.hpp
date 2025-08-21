#pragma once
#include"PackageMsgManager.hpp"
#include"MeshFinishFunctor.hpp"

// 生成mesh任务时的package
class MeshGenPackage : public PackageInfo {
public:
	// 发起mesh请求时的图片package
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