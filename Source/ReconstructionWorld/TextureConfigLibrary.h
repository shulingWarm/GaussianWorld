// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API TextureConfigLibrary
{
public:
	TextureConfigLibrary();
	~TextureConfigLibrary();

	//对texture的基本配置
	static void baseConfigTexture(UTexture2D& texture);

	//获取texture数据
	template<class DataType>
	static DataType* getTextureData(UTexture2D& texture)
	{
		//获取纹理的平台数据
		auto& platformData = *texture.GetPlatformData();
		//获取platform里面的mip数据
		auto& mip = platformData.Mips.Last();
		//锁定主体数据
		return reinterpret_cast<DataType*>(mip.BulkData.Lock(LOCK_READ_WRITE));
	}

	static void unlockTexture(UTexture2D& texture);

	//对texture的最终后处理
	static void textureFinalUpdate(UTexture2D& texture);
};
