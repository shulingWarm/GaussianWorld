#pragma once
#include"MeshSolver.hpp"
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"

class UEMeshSolver : public MeshSolver, public UE::Geometry::FMeshShapeGenerator {
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
	virtual void setVertexNum(uint32_t vertexNum) override {
		// 扩展vertex的个数
		ExtendBufferSizes(vertexNum, 0, 0, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		ExtendBufferSizes(0, faceNum, faceNum, faceNum);
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

	virtual FMeshShapeGenerator& Generate() {
		return *this;
	}
};