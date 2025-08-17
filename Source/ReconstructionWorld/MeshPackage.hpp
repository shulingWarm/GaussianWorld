#pragma once
#include"PackageMsgManager.hpp"
#include"MeshSolver.hpp"

// ���ڴ���mesh��package
class MeshPackage : public PackageInfo {
public:
	// �ڲ��洢��mesh����
	MeshSolver* meshSolver;
	// mesh�����id
	uint32_t meshTaskId;

	// ͬʱҪ�������mesh���������ĸ�ͼƬ�½��õ���
	MeshPackage(MeshSolver* meshSolver, uint32_t meshTaskId) {
		this->meshSolver = meshSolver;
		this->meshTaskId = meshTaskId;
	}
};