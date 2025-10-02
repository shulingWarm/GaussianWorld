#pragma once
#include"ImageSolver.hpp"
#include "CoreMinimal.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include"LoadFileLibrary.h"
#include"LogLibrary.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

class ArrayImage : public ImageSolver {
public:
	using ArrayType = TArray<uint8>;
	
	//solver�ڲ���ͼƬ����
	ArrayType* imgData;
	uint32_t width;
	uint32_t height;

	ArrayImage(ArrayType* imgData, uint32_t width, uint32_t height) {
		this->imgData = imgData;
		this->width = width;
		this->height = height;
	}

	ArrayImage(uint32_t width, uint32_t height) {
		imgData = new ArrayType();
		imgData->SetNum(width * height * 4);
		this->width = width;
		this->height = height;
	}

	//���ļ���ȡͼƬ�ĺ���
	ArrayImage(FString filePath) {
		auto& outStream = LogLibrary::getInstance()->fileHandle;
		//���ڴ洢��ȡ���ݵ�����
		TArray<uint8> RawFileData;
		//��ȡԭʼͼƬ�ļ����������
		if (!FFileHelper::LoadFileToArray(RawFileData, *filePath))
		{
			outStream << "Load image file ok" << std::endl;
			UE_LOG(LogTemp, Warning, TEXT("Load image file failed!"));
		}
		else {
			outStream << "Load file failed" << std::endl;
			UE_LOG(LogTemp, Warning, TEXT("Load image ok!"));
		}
		outStream << "Load 1" << std::endl;
		//׼��image warper�����module
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		outStream << "Load 2" << std::endl;
		//���ɻ�ȡͼƬ��warper
		int imageExtension = ULoadFileLibrary::getImgEnumByStr(
			ULoadFileLibrary::fStrGetSuffix(filePath)
		);
		auto imageWrapper = ImageWrapperModule.CreateImageWrapper((EImageFormat)imageExtension);
		//��ͼƬ����ע���image wrapper
		imageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num());
		//��imageWrapper����ת����BGRA����
		//����ԭʼ��bgra����
		this->imgData = new ArrayType();
		imageWrapper->GetRaw(ERGBFormat::RGBA, 8, *imgData);
		// ��¼ͼƬ�Ŀ��
		width = imageWrapper->GetWidth();
		height = imageWrapper->GetHeight();
	}

	//��ȡͼƬ�Ŀ��
	virtual uint32_t getWidth() override {
		return this->width;
	}

	virtual uint32_t getHeight() override {
		return this->height;
	}

	//��ȡĳһ�е�����ָ��
	virtual uint8_t* getRowData(uint32_t idRow) override {
		// ����ƫ����
		return &(imgData->operator[](idRow* this->width * 4));
	}

	// ����ͼƬ��ָ�����������
	virtual void setPixels(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) {
		// ȷ��һ���Ƿ��ڷ�Χ��
		if (beginId + pixelNum > getWidth() * getHeight()) {
			throw std::runtime_error("Set Pixel out of range");
		}
		// Ŀǰֻ֧��3ͨ����4ͨ���İ汾
		if (channelNum != 3 && channelNum != 4) {
			throw std::runtime_error("Invalid channel num");
		}
		// ������������ÿ������
		for (uint32_t idPixel = 0; idPixel < pixelNum; ++idPixel) {
			// ��ǰλ�õ���������
			auto currPixel = pixelData + idPixel * channelNum;
			auto imgPixelHead = &(imgData->operator[]((beginId + idPixel) * 4));
			for (uint32_t idChn = 0; idChn < channelNum; ++idChn) {
				imgPixelHead[idChn] = currPixel[idChn];
			}
			if (channelNum == 3) {
				imgPixelHead[3] = 255;
			}
		}
	}

	virtual ~ArrayImage() {
		delete this->imgData;
	}
};