#pragma once
#include"PackageMsgManager.hpp"
#include"MeshSolver.hpp"
#include"Types.hpp"

// ���ڴ���mesh��package
class MeshPackage : public PackageInfo {
public:
	// �ڲ��洢��mesh����
	Ptr<MeshSolver> meshSolver;
	// mesh�����id
	uint32_t meshTaskId;

	// ͬʱҪ�������mesh���������ĸ�ͼƬ�½��õ���
	MeshPackage(Ptr<MeshSolver> meshSolver, uint32_t meshTaskId) {
		this->meshSolver = meshSolver;
		this->meshTaskId = meshTaskId;
	}
};