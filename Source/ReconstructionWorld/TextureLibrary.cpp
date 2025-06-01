// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureLibrary.h"
#include"TextureConfigLibrary.h"
#include"TextureEffect.h"

UTexture2D* UTextureLibrary::makeWoodTexture()
{
	//纹理的宽高
	constexpr uint32 texWidth = 800;
	constexpr uint32 texHeight = 800;
	//生成一个空的texture
	UTexture2D* retTexture = UTexture2D::CreateTransient(
		texWidth, texHeight, PF_B8G8R8A8
	);
	//对texture数据的基本配置
	TextureConfigLibrary::baseConfigTexture(*retTexture);
	//获取texture数据
	auto textureData = TextureConfigLibrary::getTextureData<uint8>(*retTexture);
	//给纹理数据添加云彩效果
	TextureEffect::PSCloudEffect(textureData, texWidth, texHeight,
		TextureEffect::Vec3(19, 11, 0), TextureEffect::Vec3(50, 41, 6));
	//给云彩添加杂色
	TextureEffect::addNoise(textureData, texWidth, texHeight,
		{ 200, 200, 200 }, 0.4
	);
	//对图片做动感模糊
	TextureEffect::motionBlur(textureData, texWidth, texHeight);
	//解锁texture数据
	TextureConfigLibrary::unlockTexture(*retTexture);
	//做texture数据的收尾工作
	TextureConfigLibrary::textureFinalUpdate(*retTexture);
	return retTexture;
}
