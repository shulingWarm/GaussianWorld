#pragma once
#include"MeshSolver.hpp"

class UEMeshSolver : public MeshSolver {
public:
	//UE���½�mesh�õĲ��Ǽ򵥵�new����
	//�����⻹��Ҫ��create
	//������������õĿ���Ҳ�������߳�
	//��ʱ���½�mesh��ʱ��Ҫ�ر�ע��һ��
	static UEMeshSolver* create() {
		return new UEMeshSolver();
	}

	UEMeshSolver() {

	}

	// ���ýڵ����
	virtual void setVertexNum() override {

	}

	// ָ��ĳһ�νڵ�����
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) override {

	}
};