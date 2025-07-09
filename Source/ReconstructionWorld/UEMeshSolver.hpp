#pragma once
#include"MeshSolver.hpp"
#include"ImageSolver.hpp"
#include"ArrayImage.hpp"
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"

class UEMeshSolver : public MeshSolver, public UE::Geometry::FMeshShapeGenerator {
public:
	// �ڲ����������ݽṹ
	ImageSolver* texture = nullptr;

	//UE���½�mesh�õĲ��Ǽ򵥵�new����
	//�����⻹��Ҫ��create
	//������������õĿ���Ҳ�������߳�
	//��ʱ���½�mesh��ʱ��Ҫ�ر�ע��һ��
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

	// ��ȡtexture�Ĵ�С
	// ����0��ʱ���ȡwidth������1��ʱ���ȡheight
	virtual uint32_t getTextureSize(uint32_t dim) {
		if (dim == 0)
			return this->texture->getWidth();
		return this->texture->getHeight();
	}

	// ���ýڵ����
	virtual void setVertexNum(uint32_t vertexNum) override {
		// ��չvertex�ĸ���
		ExtendBufferSizes(vertexNum, 0, 0, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		ExtendBufferSizes(0, faceNum, faceNum, faceNum);
	}

	virtual void setTextureShape(uint32_t width, uint32_t height) {
		// ����ֻ�ܱ�����һ��
		if (texture != nullptr) {
			throw std::runtime_error("Texture should not be initialized twice.");
		}
		// ��ʼ��texture 
		this->texture = new ArrayImage(width, height);
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

	virtual void setVertexUvSequence(uint32_t beginId, uint32_t vertexUvNum, float* vertexUvData) override {
		// �������е�vertex uv
		for (uint32_t idVertex = 0; idVertex < vertexUvNum; ++idVertex) {
			// ��ǰ��vertexͷָ��
			auto currVertexData = vertexUvData + idVertex * 2;
			// д�뵱ǰ��vertex uv
			SetUV(idVertex + beginId, { currVertexData[0], currVertexData[1] }, idVertex + beginId);
		}
	}

	virtual void setFaceUvSequence(uint32_t beginId, uint32_t faceUvNum, int* faceUvData) override {
		for (uint32_t idFace = 0; idFace < faceUvNum; ++idFace) {
			auto currFaceData = faceUvData + 3 * idFace;
			SetTriangleUVs(idFace, { currFaceData[0], currFaceData[1], currFaceData[2] });
		}
	}

	// ����texture������
	virtual void setTexturePixelSequence(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) {
		// �����ڲ�texture������pixel����
		this->texture->setPixels(beginId, pixelNum, channelNum, pixelData);
	}

	virtual FMeshShapeGenerator& Generate() {
		return *this;
	}
};