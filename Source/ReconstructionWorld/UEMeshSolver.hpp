#pragma once
#include"MeshSolver.hpp"
#include"ImageSolver.hpp"
#include"ArrayImage.hpp"
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"

class UEMeshSolver : public MeshSolver, public UE::Geometry::FMeshShapeGenerator {
public:
	// 内部的纹理数据结构
	ImageSolver* texture = nullptr;

	//UE的新建mesh用的不是简单的new函数
	//所以这还是要用create
	//而且这个东西用的可能也不是主线程
	//到时候新建mesh的时候要特别注意一下
	static UEMeshSolver* create() {
		return new UEMeshSolver();
	}

	UEMeshSolver() {

	}

	virtual uint32_t getFaceNum() {
		return this->Triangles.Num();
	}

	virtual uint32_t getVertexNum() {
		return this->Vertices.Num();
	}

	virtual uint32_t getVertexUvNum() {
		return this->UVs.Num();
	}

	// 获取texture的大小
	// 传入0的时候获取width，传入1的时候获取height
	virtual uint32_t getTextureSize(uint32_t dim) {
		if (dim == 0)
			return this->texture->getWidth();
		return this->texture->getHeight();
	}

	// 设置节点个数
	virtual void setVertexNum(uint32_t vertexNum) override {
		// 扩展vertex的个数
		ExtendBufferSizes(vertexNum, 0, 0, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		ExtendBufferSizes(0, faceNum, faceNum, faceNum);
	}

	virtual void setTextureShape(uint32_t width, uint32_t height) {
		// 纹理只能被设置一次
		if (texture != nullptr) {
			throw std::runtime_error("Texture should not be initialized twice.");
		}
		// 初始化texture 
		this->texture = new ArrayImage(width, height);
	}

	// 指定某一段节点数据
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) override {
		// 记录节点数据
		for (uint32_t idVertex = 0; idVertex < vertexNum; ++idVertex) {
			// 当前坐标的起始点
			float* currVertex = vertexData + idVertex * 3;
			SetVertex(idVertex + beginId, { currVertex[0], currVertex[1], currVertex[2] });
		}
	}

	// 设置某一段的face数据
	virtual void setFaceSequence(uint32_t beginId, uint32_t faceNum, int* faceData) override {
		// 设置面数据
		for (uint32_t idFace = 0; idFace < faceNum; ++idFace) {
			// 当前位置的面
			int* currFace = faceData + idFace * 3;
			SetTriangle(idFace + beginId, currFace[0], currFace[1], currFace[2]);
		}
	}

	virtual void setVertexUvSequence(uint32_t beginId, uint32_t vertexUvNum, float* vertexUvData) override {
		// 遍历所有的vertex uv
		for (uint32_t idVertex = 0; idVertex < vertexUvNum; ++idVertex) {
			// 当前的vertex头指针
			auto currVertexData = vertexUvData + idVertex * 2;
			// 写入当前的vertex uv
			SetUV(idVertex + beginId, { currVertexData[0], currVertexData[1] }, idVertex + beginId);
		}
	}

	virtual void setFaceUvSequence(uint32_t beginId, uint32_t faceUvNum, int* faceUvData) override {
		for (uint32_t idFace = 0; idFace < faceUvNum; ++idFace) {
			auto currFaceData = faceUvData + 3 * idFace;
			SetTriangleUVs(idFace, { currFaceData[0], currFaceData[1], currFaceData[2] });
		}
	}

	// 设置texture的序列
	virtual void setTexturePixelSequence(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) {
		// 调用内部texture来设置pixel内容
		this->texture->setPixels(beginId, pixelNum, channelNum, pixelData);
	}

	virtual FMeshShapeGenerator& Generate() {
		return *this;
	}
};