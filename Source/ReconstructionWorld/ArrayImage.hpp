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
	
	//solver内部的图片数据
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

	//从文件读取图片的函数
	ArrayImage(FString filePath) {
		auto& outStream = LogLibrary::getInstance()->fileHandle;
		//用于存储读取数据的数组
		TArray<uint8> RawFileData;
		//读取原始图片文件里面的数据
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
		//准备image warper里面的module
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		outStream << "Load 2" << std::endl;
		//生成获取图片的warper
		int imageExtension = ULoadFileLibrary::getImgEnumByStr(
			ULoadFileLibrary::fStrGetSuffix(filePath)
		);
		auto imageWrapper = ImageWrapperModule.CreateImageWrapper((EImageFormat)imageExtension);
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

	// 设置图片的指定区域的像素
	virtual void setPixels(uint32_t beginId, uint32_t pixelNum,
		uint32_t channelNum, uint8_t* pixelData) {
		// 确认一下是否在范围内
		if (beginId + pixelNum > getWidth() * getHeight()) {
			throw std::runtime_error("Set Pixel out of range");
		}
		// 目前只支持3通道和4通道的版本
		if (channelNum != 3 && channelNum != 4) {
			throw std::runtime_error("Invalid channel num");
		}
		// 遍历传进来的每个像素
		for (uint32_t idPixel = 0; idPixel < pixelNum; ++idPixel) {
			// 当前位置的像素数据
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