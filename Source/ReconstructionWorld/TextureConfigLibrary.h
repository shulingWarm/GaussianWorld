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

	//��texture�Ļ�������
	static void baseConfigTexture(UTexture2D& texture);

	//��ȡtexture����
	template<class DataType>
	static DataType* getTextureData(UTexture2D& texture)
	{
		//��ȡ�����ƽ̨����
		auto& platformData = *texture.GetPlatformData();
		//��ȡplatform�����mip����
		auto& mip = platformData.Mips.Last();
		//������������
		return reinterpret_cast<DataType*>(mip.BulkData.Lock(LOCK_READ_WRITE));
	}

	static void unlockTexture(UTexture2D& texture);

	//��texture�����պ���
	static void textureFinalUpdate(UTexture2D& texture);
};
