#pragma once
#include"MeshSolver.hpp"

class UEMeshSolver : public MeshSolver {
public:
	//UE的新建mesh用的不是简单的new函数
	//所以这还是要用create
	//而且这个东西用的可能也不是主线程
	//到时候新建mesh的时候要特别注意一下
	static UEMeshSolver* create() {
		return new UEMeshSolver();
	}

	UEMeshSolver() {

	}

	// 设置节点个数
	virtual void setVertexNum() override {

	}

	// 指定某一段节点数据
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) override {

	}
};