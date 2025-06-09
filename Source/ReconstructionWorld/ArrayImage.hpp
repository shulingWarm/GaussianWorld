#pragma once
#include"ImageSolver.hpp"
#include "CoreMinimal.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

class ArrayImage : public ImageSolver {
public:
	using ArrayType = TArray<uint8>;
	
	//solver内部的图片数据
	ArrayType* imgData;
	uint32_t width;
	uint32_t height;

	ArrayImage(ArrayType* imgData, uint32_t width, uint32_t height) {
		this->imgData = imgData;
		this->width = width;
		this->height = height;
	}

	//从文件读取图片的函数
	ArrayImage(FString filePath) {
		//用于存储读取数据的数组
		TArray<uint8> RawFileData;
		//读取原始图片文件里面的数据
		if (!FFileHelper::LoadFileToArray(RawFileData, *filePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load image file failed!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Load image ok!"));
		}
		//准备image warper里面的module
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		//生成获取图片的warper
		auto imageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		//把图片数据注册进image wrapper
		imageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num());
		//从imageWrapper里面转换成BGRA数据
		//这是原始的bgra数据
		this->imgData = new ArrayType();
		imageWrapper->GetRaw(ERGBFormat::RGBA, 8, *imgData);
		// 记录图片的宽高
		width = imageWrapper->GetWidth();
		height = imageWrapper->GetHeight();
	}

	//获取图片的宽高
	virtual uint32_t getWidth() override {
		return this->width;
	}

	virtual uint32_t getHeight() override {
		return this->height;
	}

	//获取某一行的数据指针
	virtual uint8_t* getRowData(uint32_t idRow) override {
		// 计算偏移量
		return &(imgData->operator[](idRow* this->width * 4));
	}
};