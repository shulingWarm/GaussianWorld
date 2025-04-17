// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureConfigLibrary.h"

TextureConfigLibrary::TextureConfigLibrary()
{
}

TextureConfigLibrary::~TextureConfigLibrary()
{

}

void TextureConfigLibrary::baseConfigTexture(UTexture2D& texture)
{
	//采样方式改成最近采样
	texture.Filter = TextureFilter::TF_Nearest;
	texture.CompressionSettings = TextureCompressionSettings::TC_HDR;
	texture.SRGB = false;
	texture.NeverStream = true;
	auto platformData = texture.GetPlatformData();
	platformData->SetNumSlices(1);
}

void TextureConfigLibrary::unlockTexture(UTexture2D& texture)
{
	texture.GetPlatformData()->Mips.Last().BulkData.Unlock();
}

void TextureConfigLibrary::textureFinalUpdate(UTexture2D& texture)
{
	texture.UpdateResource();
	texture.AddToRoot();
}
