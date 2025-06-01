// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureLibrary.h"
#include"TextureConfigLibrary.h"
#include"TextureEffect.h"

UTexture2D* UTextureLibrary::makeWoodTexture()
{
	//����Ŀ��
	constexpr uint32 texWidth = 800;
	constexpr uint32 texHeight = 800;
	//����һ���յ�texture
	UTexture2D* retTexture = UTexture2D::CreateTransient(
		texWidth, texHeight, PF_B8G8R8A8
	);
	//��texture���ݵĻ�������
	TextureConfigLibrary::baseConfigTexture(*retTexture);
	//��ȡtexture����
	auto textureData = TextureConfigLibrary::getTextureData<uint8>(*retTexture);
	//��������������Ʋ�Ч��
	TextureEffect::PSCloudEffect(textureData, texWidth, texHeight,
		TextureEffect::Vec3(19, 11, 0), TextureEffect::Vec3(50, 41, 6));
	//���Ʋ������ɫ
	TextureEffect::addNoise(textureData, texWidth, texHeight,
		{ 200, 200, 200 }, 0.4
	);
	//��ͼƬ������ģ��
	TextureEffect::motionBlur(textureData, texWidth, texHeight);
	//����texture����
	TextureConfigLibrary::unlockTexture(*retTexture);
	//��texture���ݵ���β����
	TextureConfigLibrary::textureFinalUpdate(*retTexture);
	return retTexture;
}
