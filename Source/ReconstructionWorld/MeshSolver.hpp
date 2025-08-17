#pragma once
#include<iostream>
#include<string>
#include "Generators/MeshShapeGenerator.h"

// mesh�ĳ���ӿ�
class MeshSolver {
public:
	// ���ýڵ����
	virtual void setVertexNum(uint32_t vertexNum) = 0;

	// ����face�ĸ���
	virtual void setFaceNum(uint32_t vertexNum) = 0;

	// ���������shape
	virtual void setTextureShape(uint32_t width, uint32_t height) = 0;

	// ��ȡface�ĸ���
	virtual uint32_t getFaceNum() = 0;
	virtual uint32_t getVertexNum() = 0;
	virtual uint32_t getVertexUvNum() = 0;

	// ָ��ĳһ�νڵ�����
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) = 0;

	// ����ĳһ�ε�face����
	virtual void setFaceSequence(uint32_t beginId, uint32_t faceNum, int* faceData) = 0;

	// ����vertex uv sequence
	virtual void setVertexUvSequence(uint32_t beginId, uint32_t vertexUvNum, float* vertexUvData) = 0;

	// ����face uv sequence
	virtual void setFaceUvSequence(uint32_t beginId, uint32_t faceUvNum, int* faceUvData) = 0;

	// ���������texture����
	virtual void setTexturePixelSequence(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) = 0;

	// ��ȡtexture�Ĵ�С
	// ����0��ʱ���ȡwidth������1��ʱ���ȡheight
	virtual uint32_t getTextureSize(uint32_t dim) = 0;

	// ��mesh���浽ָ��·�� 
	virtual void saveMesh(std::string path) = 0;

	// ����ue mesh
	virtual void buildUeMesh(UDynamicMesh* mesh) = 0;

	// �������ȡue texture
	virtual UTexture2D* makeUeTexture() = 0;
};