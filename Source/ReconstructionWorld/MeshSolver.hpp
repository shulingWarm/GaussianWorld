#pragma once
#include<iostream>

// mesh�ĳ���ӿ�
class MeshSolver {
public:
	// ���ýڵ����
	virtual void setVertexNum(uint32_t vertexNum) = 0;

	// ����face�ĸ���
	virtual void setFaceNum(uint32_t vertexNum) = 0;

	// ָ��ĳһ�νڵ�����
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) = 0;

	// ����ĳһ�ε�face����
	virtual void setFaceSequence(uint32_t beginId, uint32_t faceNum, int* faceData) = 0;
};