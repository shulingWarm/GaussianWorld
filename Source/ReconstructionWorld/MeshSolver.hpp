#pragma once
#include<iostream>

// mesh的抽象接口
class MeshSolver {
public:
	// 设置节点个数
	virtual void setVertexNum() = 0;

	// 指定某一段节点数据
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) = 0;
};