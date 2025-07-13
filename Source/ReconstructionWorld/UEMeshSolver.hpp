#pragma once
#include"MeshSolver.hpp"
#include"ImageSolver.hpp"
#include"ArrayImage.hpp"
#include<fstream>
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"
#include"MeshLibrary.h"

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
		// ��չvertex�ĸ��� ����ÿ���ڵ�ֻ��һ��uv
		SetBufferSizes(vertexNum, 0, vertexNum, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		SetBufferSizes(0, faceNum, 0, 0);
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
			SetVertex(idVertex + beginId, { currVertex[0]*100, -currVertex[2]*100, -currVertex[1]*100 });
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

	virtual void saveMesh(std::string path) override {
		std::fstream fileHandle(path, std::ios::out | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot open file");
		}
		// ��¼�ڵ�ĸ���
		uint32_t verticeNum = Vertices.Num();
		uint32_t faceNum = Triangles.Num();
		// �ڵ�uv�ĸ���
		uint32_t vertexUvNum = UVs.Num();
		uint32_t faceUvNum = TriangleUVs.Num();
		// ����Ŀ��
		uint32_t textureShape[] = {
			getTextureSize(0), getTextureSize(1)
		};
		fileHandle.write((char*)&verticeNum, sizeof(uint32_t));
		fileHandle.write((char*)&faceNum, sizeof(uint32_t));
		fileHandle.write((char*)&vertexUvNum, sizeof(uint32_t));
		fileHandle.write((char*)&faceUvNum, sizeof(uint32_t));
		fileHandle.write((char*)textureShape, sizeof(uint32_t) * 2);
		// ��¼vertex ����
		fileHandle.write((char*)Vertices.GetData(), sizeof(FVector3d) * verticeNum);
		fileHandle.write((char*)Triangles.GetData(), sizeof(UE::Geometry::FIndex3i) * faceNum);
		fileHandle.write((char*)UVs.GetData(), sizeof(FVector2f) * vertexUvNum);
		fileHandle.write((char*)TriangleUVs.GetData(), sizeof(UE::Geometry::FIndex3i) * faceUvNum);
		// ��¼��������
		fileHandle.write((char*)texture->getRowData(0),
			sizeof(uint8_t) * texture->getWidth() * texture->getHeight() * 4);
		fileHandle.close();
	}

	// ���ļ��м�������
	void loadFromFile(std::string filePath) {
		std::fstream fileHandle;
		fileHandle.open(filePath, std::ios::in | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot load file");
		}
		// ��ȡ�ڵ���� ���ζ�Ӧvertex face vertex_uv face_uv
		uint32_t meshProperty[4];
		fileHandle.read((char*)meshProperty, sizeof(uint32_t) * 4);
		// ��ȡtexture������
		uint32_t textureSize[2];
		fileHandle.read((char*)textureSize, sizeof(uint32_t) * 2);
		// ���ýڵ����
		setVertexNum(meshProperty[0]);
		// ����face���� ������ʱ������face����uv�ĸ���������ͬ��
		setFaceNum(meshProperty[1]);
		// ����texture
		setTextureShape(textureSize[0], textureSize[1]);
		// ��ȡ���е�vertice����
		fileHandle.read((char*)Vertices.GetData(), sizeof(FVector3d) * meshProperty[0]);
		fileHandle.read((char*)Triangles.GetData(), sizeof(UE::Geometry::FIndex3i) * meshProperty[1]);
		fileHandle.read((char*)UVs.GetData(), sizeof(FVector2f) * meshProperty[2]);
		fileHandle.read((char*)TriangleUVs.GetData(), sizeof(UE::Geometry::FIndex3i) * meshProperty[3]);
		// ��¼��������
		fileHandle.read((char*)texture->getRowData(0),
			sizeof(uint8_t) * texture->getWidth() * texture->getHeight() * 4);
		fileHandle.close();
	}

	// ����ue mesh
	virtual void buildUeMesh(UDynamicMesh* mesh) override {
		// ��������hard coding�ķ�ʽ������
		for (int idVertex = 0; idVertex < Vertices.Num(); ++idVertex) {
			auto& vertex = Vertices[idVertex];
			// ���Ѿ���y,z����ȡ�����İ汾��
			// ��z����������
			vertex[2] = -vertex[2];
			// ��x����Ҳȡ����
			vertex[0] = -vertex[0];
		}
		// ���meshԭ��
		MeshLibrary::addMeshPrimitive(mesh, this, FTransform(), FVector3d::Zero());
	}
};