#pragma once
#include"ImageSolver.hpp"
#include "CoreMinimal.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
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

	//���ļ���ȡͼƬ�ĺ���
	ArrayImage(FString filePath) {
		//���ڴ洢��ȡ���ݵ�����
		TArray<uint8> RawFileData;
		//��ȡԭʼͼƬ�ļ����������
		if (!FFileHelper::LoadFileToArray(RawFileData, *filePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load image file failed!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Load image ok!"));
		}
		//׼��image warper�����module
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		//���ɻ�ȡͼƬ��warper
		auto imageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
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
};