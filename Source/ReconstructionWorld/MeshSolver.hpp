#pragma once
#include<iostream>

// mesh�ĳ���ӿ�
class MeshSolver {
public:
	// ���ýڵ����
	virtual void setVertexNum() = 0;

	// ָ��ĳһ�νڵ�����
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) = 0;
};