#pragma once
#include"PackageMsgManager.hpp"
#include"MeshSolver.hpp"

// ���ڴ���mesh��package
class MeshPackage : public PackageInfo {
public:
	// �ڲ��洢��mesh����
	MeshSolver* meshSolver;

	// ͬʱҪ�������mesh���������ĸ�ͼƬ�½��õ���
	MeshPackage(MeshSolver* meshSolver, uint32_t oriImagePackage) {
		this->meshSolver = meshSolver;
	}
};