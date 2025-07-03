#pragma once
#include"MeshSolver.hpp"
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"

class UEMeshSolver : public MeshSolver, public UE::Geometry::FMeshShapeGenerator {
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
	virtual void setVertexNum(uint32_t vertexNum) override {
		// ��չvertex�ĸ���
		ExtendBufferSizes(vertexNum, 0, 0, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		ExtendBufferSizes(0, faceNum, faceNum, faceNum);
	}

	// ָ��ĳһ�νڵ�����
	virtual void setVertexSequence(uint32_t beginId, uint32_t vertexNum, float* vertexData) override {
		// ��¼�ڵ�����
		for (uint32_t idVertex = 0; idVertex < vertexNum; ++idVertex) {
			// ��ǰ�������ʼ��
			float* currVertex = vertexData + idVertex * 3;
			SetVertex(idVertex + beginId, { currVertex[0], currVertex[1], currVertex[2] });
		}
	}

	// ����ĳһ�ε�face����
	virtual void setFaceSequence(uint32_t beginId, uint32_t faceNum, int* faceData) override {
		// ����������
		for (uint32_t idFace = 0; idFace < faceNum; ++idFace) {
			// ��ǰλ�õ���
			int* currFace = faceData + idFace * 3;
			SetTriangle(idFace + beginId, currFace[0], currFace[1], currFace[2]);
		}
	}

	virtual FMeshShapeGenerator& Generate() {
		return *this;
	}
};