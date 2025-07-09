#pragma once
#include<iostream>

// mesh的抽象接口
class MeshSolver {
public:
	// 设置节点个数
	virtual void setVertexNum(uint32_t vertexNum) = 0;

	// 设置face的个数
	virtual void setFaceNum(uint32_t vertexNum) = 0;

	// 设置纹理的shape
	virtual void setTextureShape(uint32_t width, uint32_t height) = 0;

	// 获取face的个数
	virtual uint32_t getFaceNum() = 0;
	virtual uint32_t getVertexNum() = 0;
	virtual uint32_t getVertexUvNum() = 0;

	// 指定某一段节点数据
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) = 0;

	// 设置某一段的face数据
	virtual void setFaceSequence(uint32_t beginId, uint32_t faceNum, int* faceData) = 0;

	// 设置vertex uv sequence
	virtual void setVertexUvSequence(uint32_t beginId, uint32_t vertexUvNum, float* vertexUvData) = 0;

	// 设置face uv sequence
	virtual void setFaceUvSequence(uint32_t beginId, uint32_t faceUvNum, int* faceUvData) = 0;

	// 设置纹理的texture序列
	virtual void setTexturePixelSequence(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) = 0;

	// 获取texture的大小
	// 传入0的时候获取width，传入1的时候获取height
	virtual uint32_t getTextureSize(uint32_t dim) = 0;
};