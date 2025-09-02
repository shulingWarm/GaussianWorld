#pragma once
#include"PackageMsgManager.hpp"
#include"MeshSolver.hpp"
#include"Types.hpp"

// 用于传输mesh的package
class MeshPackage : public PackageInfo {
public:
	// 内部存储的mesh对象
	Ptr<MeshSolver> meshSolver;
	// mesh任务的id
	uint32_t meshTaskId;

	// 同时要传入这个mesh当初是由哪个图片新建得到的
	MeshPackage(Ptr<MeshSolver> meshSolver, uint32_t meshTaskId) {
		this->meshSolver = meshSolver;
		this->meshTaskId = meshTaskId;
	}
};