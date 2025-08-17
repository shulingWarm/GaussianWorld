#pragma once
#include"MeshSolver.hpp"
#include"ImageSolver.hpp"
#include"ArrayImage.hpp"
#include"TextureConfigLibrary.h"
#include<fstream>
#include "CoreMinimal.h"
#include "Generators/MeshShapeGenerator.h"
#include"MeshLibrary.h"
#include"LogLibrary.h"

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
		// 扩展vertex的个数 假设每个节点只有一个uv
		SetBufferSizes(vertexNum, 0, vertexNum, 0);
	}

	virtual void setFaceNum(uint32_t faceNum) override {
		SetBufferSizes(0, faceNum, 0, 0);
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
			SetVertex(idVertex + beginId, { currVertex[0]*100, -currVertex[2]*100, -currVertex[1]*100 });
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
			SetTrianglePolygon(idFace, idFace);
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

	virtual void saveMesh(std::string path) override {
		std::fstream fileHandle(path, std::ios::out | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot open file");
		}
		// 记录节点的个数
		uint32_t verticeNum = Vertices.Num();
		uint32_t faceNum = Triangles.Num();
		// 节点uv的个数
		uint32_t vertexUvNum = UVs.Num();
		uint32_t faceUvNum = TriangleUVs.Num();
		// 纹理的宽高
		uint32_t textureShape[] = {
			getTextureSize(0), getTextureSize(1)
		};
		fileHandle.write((char*)&verticeNum, sizeof(uint32_t));
		fileHandle.write((char*)&faceNum, sizeof(uint32_t));
		fileHandle.write((char*)&vertexUvNum, sizeof(uint32_t));
		fileHandle.write((char*)&faceUvNum, sizeof(uint32_t));
		fileHandle.write((char*)textureShape, sizeof(uint32_t) * 2);
		// 记录vertex 数据
		fileHandle.write((char*)Vertices.GetData(), sizeof(FVector3d) * verticeNum);
		fileHandle.write((char*)Triangles.GetData(), sizeof(UE::Geometry::FIndex3i) * faceNum);
		fileHandle.write((char*)UVs.GetData(), sizeof(FVector2f) * vertexUvNum);
		fileHandle.write((char*)TriangleUVs.GetData(), sizeof(UE::Geometry::FIndex3i) * faceUvNum);
		// 记录纹理数据
		fileHandle.write((char*)texture->getRowData(0),
			sizeof(uint8_t) * texture->getWidth() * texture->getHeight() * 4);
		fileHandle.close();
	}

	// 从文件中加载数据
	void loadFromFile(std::string filePath) {
		std::fstream fileHandle;
		fileHandle.open(filePath, std::ios::in | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot load file");
		}
		// 读取节点个数 依次对应vertex face vertex_uv face_uv
		uint32_t meshProperty[4];
		fileHandle.read((char*)meshProperty, sizeof(uint32_t) * 4);
		// 读取texture的数量
		uint32_t textureSize[2];
		fileHandle.read((char*)textureSize, sizeof(uint32_t) * 2);
		// 设置节点个数
		setVertexNum(meshProperty[0]);
		// 设置face个数 这里暂时假设了face里面uv的个数都是相同的
		setFaceNum(meshProperty[1]);
		// 设置texture
		setTextureShape(textureSize[0], textureSize[1]);
		// 读取所有的vertice数据
		fileHandle.read((char*)Vertices.GetData(), sizeof(FVector3d) * meshProperty[0]);
		fileHandle.read((char*)Triangles.GetData(), sizeof(UE::Geometry::FIndex3i) * meshProperty[1]);
		fileHandle.read((char*)UVs.GetData(), sizeof(FVector2f) * meshProperty[2]);
		fileHandle.read((char*)TriangleUVs.GetData(), sizeof(UE::Geometry::FIndex3i) * meshProperty[3]);
		// 记录纹理数据
		fileHandle.read((char*)texture->getRowData(0),
			sizeof(uint8_t) * texture->getWidth() * texture->getHeight() * 4);
		// 把uv的parent vertex依次设置
		for (int idVertex = 0; idVertex < UVParentVertex.Num(); ++idVertex) {
			UVParentVertex[idVertex] = idVertex;
		}
		fileHandle.close();
	}

	// 打印mesh的uv信息
	void printMeshUv() {
		// 获取log的数据流
		auto& stream = LogLibrary::getInstance()->output();
		// 读取前100个vertex uv
		for (uint32_t idVertex = 2000; idVertex < 2100; ++idVertex) {
			// 获取当前位置的vertex uv
			auto& vertexUv = UVs[idVertex];
			// 打印vertex信息
			stream << vertexUv.X << " " << vertexUv.Y << std::endl;
		}

		// 打印100个face uv
		for (uint32_t idFace = 0; idFace < 100; ++idFace) {
			auto& faceUv = TriangleUVs[idFace];
			stream << faceUv[0] << " " << faceUv[1] << " " << faceUv[2] << std::endl;
		}
	}

	// 构建ue mesh
	virtual void buildUeMesh(UDynamicMesh* mesh) override {
		// 读取关注的face
		std::fstream fileHandle("E:/temp/focus.txt", std::ios::in);
		int focusMin, focusMax;
		fileHandle >> focusMin >> focusMax;
		auto& stream = LogLibrary::getInstance()->output();
		// 在这里用hard coding的方式做缩放
		for (int idVertex = 0; idVertex < Vertices.Num(); ++idVertex) {
			auto& vertex = Vertices[idVertex];
			// 这已经是y,z坐标取过反的版本了
			// 把z坐标正过来
			vertex[2] = -vertex[2];
			// 给x坐标也取个反
			vertex[0] = -vertex[0];
		}
		uint32_t textureWidth = getTextureSize(0);
		uint32_t textureHeight = getTextureSize(1);
		// 把所有的vertex uv处理一下
		for (int idUv = 0; idUv < UVs.Num(); ++idUv) {
			auto& currUv = UVs[idUv];
			currUv.Y = textureHeight - currUv.Y;
		}
		//stream << "Face and focus: " << TriangleUVs.Num() << std::endl;
		//// 纹理的宽高
		
		//// 除了关注的face，其他都跳过
		//for (int idFace = 0; idFace < TriangleUVs.Num(); ++idFace) {
		//	auto& faceUv = TriangleUVs[idFace];
		//	// 判断是不是关注的face
		//	if (idFace >= focusMin && idFace <= focusMax) {
		//		// 处理 face
		//		stream << "face id: "<< idFace << std::endl;
		//		stream << faceUv[0] << " " << faceUv[1] << " " << faceUv[2] << std::endl;
		//		for (int idDim = 0; idDim < 3; ++idDim) {
		//			auto& vertexUv = UVs[faceUv[idDim]];
		//			stream << "(" << vertexUv[0]*textureWidth << "," << vertexUv[1]*textureHeight << ") ";
		//		}
		//		stream << std::endl;
		//	}
		//	else {
		//		faceUv[0] = 0;
		//		faceUv[1] = 1;
		//		faceUv[2] = 2;
		//	}
		//}
		// 添加mesh原语
		MeshLibrary::addMeshPrimitive(mesh, this, FTransform(), FVector3d::Zero());
	}

	// 从里面获取ue texture
	virtual UTexture2D* makeUeTexture() override {
		// texture的宽高
		uint32_t textureWidth = this->getTextureSize(0);
		uint32_t textureHeight = this->getTextureSize(1);
		//生成一个空的texture
		UTexture2D* retTexture = UTexture2D::CreateTransient(
			textureWidth, textureHeight, PF_B8G8R8A8
		);
		// 完成对纹理的基本配置
		TextureConfigLibrary::baseConfigTexture(*retTexture);
		//获取texture数据
		auto textureData = TextureConfigLibrary::getTextureData<uint8>(*retTexture);
		auto oriTextureData = this->texture->getRowData(0);
		// 把本地里面的texture数据复制进去
		// 先假设这里面是BGRA，如果后续数据位置不对的话再调整
		uint32_t pixelNum = textureWidth * textureHeight;
		for (uint32_t idPixel = 0; idPixel < pixelNum; ++idPixel) {
			// 当前像素的数据
			auto srcData = oriTextureData + idPixel * 4;
			auto dstData = textureData + idPixel * 4;
			// 把rgb颜色翻转成bgr颜色
			dstData[0] = srcData[2];
			dstData[1] = srcData[1];
			dstData[2] = srcData[0];
			dstData[3] = srcData[3];
		}
		//解锁texture数据
		TextureConfigLibrary::unlockTexture(*retTexture);
		//做texture数据的收尾工作
		TextureConfigLibrary::textureFinalUpdate(*retTexture);
		return retTexture;
	}
};