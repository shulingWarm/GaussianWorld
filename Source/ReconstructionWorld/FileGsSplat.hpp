#pragma once
#include"GaussianSplatSolver.hpp"
#include<string>
#include<fstream>
#include"GsSplatLib.hpp"
#include"GaussianDescriptor.h"
#include"TextureConfigLibrary.h"

// 纯文件形式的高斯solver
class FileGsSplat : public GaussianSplatSolver {
public:
	std::string filePath;

	FileGsSplat(std::string filePath) {
		this->filePath = filePath;
	}

	virtual void fillGsScene(UGaussianDescriptor* gsDesc) override {
		// 读取文件流
		std::ifstream fileHandle(filePath, std::ios::out | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot open gaussian file");
		}
		// 准备纹理空间里面需要用到的纹理
		auto& retTexture = gsDesc->textreList;
		// 读取ply header
		PlyHeader plyHeader;
		GsSplatLib::loadPlyHeader(fileHandle, plyHeader);
		// 准备纹理的大小
		auto texSize = GsSplatLib::getTextureSize(plyHeader.verticeNum);
		// 指定计算结果里面的gaussian num
		gsDesc->gaussianNum = plyHeader.verticeNum;
		//读取texture里面的数据
		FFloat16* textureData[4];
		//初始化4个texture
		for (int idTexture = 0; idTexture < 4; ++idTexture)
		{
			//初始化一个临时的texture
			auto tempTexture = UTexture2D::CreateTransient(
				texSize.X, texSize.Y, PF_FloatRGBA
			);
			//对纹理数据的基础配置
			TextureConfigLibrary::baseConfigTexture(*tempTexture);
			//记录texture数据
			textureData[idTexture] = TextureConfigLibrary::getTextureData<FFloat16>(*tempTexture);
			//添加到texture的列表里面
			retTexture.Add(tempTexture);
		}
		//把数据读取进texture里面
		GsSplatLib::assignTextureData(fileHandle, textureData[0], textureData[1], textureData[2], textureData[3],
			plyHeader.verticeNum, retTexture[0]->GetSizeX() * retTexture[0]->GetSizeY());
		//遍历texture数据，解锁数据
		for (int idTexture = 0; idTexture < 4; ++idTexture)
		{
			TextureConfigLibrary::unlockTexture(retTexture[idTexture][0]);
			TextureConfigLibrary::textureFinalUpdate(retTexture[idTexture][0]);
		}
		//指定descriptor的采样大小
		gsDesc->sampleSizeX = retTexture[0]->GetSizeX();
		gsDesc->sampleSizeY = gsDesc->gaussianNum / gsDesc->sampleSizeX + 1;
		//指定采样步长
		gsDesc->sampleStep[0] = 1.f / (retTexture[0]->GetSizeX() + 0.5f);
		gsDesc->sampleStep[1] = 1.f / (retTexture[0]->GetSizeY());
		fileHandle.close();
	}
};