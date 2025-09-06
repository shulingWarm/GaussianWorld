#pragma once
#include"GaussianSplatSolver.hpp"
#include<string>
#include<fstream>
#include"GsSplatLib.hpp"
#include"GaussianDescriptor.h"
#include"TextureConfigLibrary.h"

// ���ļ���ʽ�ĸ�˹solver
class FileGsSplat : public GaussianSplatSolver {
public:
	std::string filePath;

	FileGsSplat(std::string filePath) {
		this->filePath = filePath;
	}

	virtual void fillGsScene(UGaussianDescriptor* gsDesc) override {
		// ��ȡ�ļ���
		std::fstream fileHandle(filePath, std::ios::out | std::ios::binary);
		if (!fileHandle.is_open()) {
			throw std::runtime_error("Cannot open gaussian file");
		}
		// ׼������ռ�������Ҫ�õ�������
		auto& retTexture = gsDesc->textreList;
		// ��ȡply header
		PlyHeader plyHeader;
		GsSplatLib::loadPlyHeader(fileHandle, plyHeader);
		// ׼������Ĵ�С
		auto texSize = GsSplatLib::getTextureSize(plyHeader.verticeNum);
		// ָ�������������gaussian num
		gsDesc->gaussianNum = plyHeader.verticeNum;
		//��ȡtexture���������
		FFloat16* textureData[4];
		//��ʼ��4��texture
		for (int idTexture = 0; idTexture < 4; ++idTexture)
		{
			//��ʼ��һ����ʱ��texture
			auto tempTexture = UTexture2D::CreateTransient(
				texSize.X, texSize.Y, PF_FloatRGBA
			);
			//���������ݵĻ�������
			baseConfigTexture(*tempTexture);
			//��¼texture����
			textureData[idTexture] = getTextureData<FFloat16>(*tempTexture);
			//��ӵ�texture���б�����
			retTexture.Add(tempTexture);
		}
		//�����ݶ�ȡ��texture����
		assignTextureData(fileHandle, textureData[0], textureData[1], textureData[2], textureData[3],
			header.verticeNum, retTexture[0]->GetSizeX() * retTexture[0]->GetSizeY());
		//����texture���ݣ���������
		for (int idTexture = 0; idTexture < 4; ++idTexture)
		{
			unlockTexture(retTexture[idTexture][0]);
			textureFinalUpdate(retTexture[idTexture][0]);
		}
		//ָ��descriptor�Ĳ�����С
		descriptor->sampleSizeX = retTexture[0]->GetSizeX();
		descriptor->sampleSizeY = descriptor->gaussianNum / descriptor->sampleSizeX + 1;
		//ָ����������
		descriptor->sampleStep[0] = 1.f / (retTexture[0]->GetSizeX() + 0.5f);
		descriptor->sampleStep[1] = 1.f / (retTexture[0]->GetSizeY());
		fileHandle.close();
	}
};