// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureLibrary.h"
#include"TextureConfigLibrary.h"
#include"ImageUtils.h"
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

static bool ExprotUTexture2D(UTexture2D* Img, const FString& fileDestination)
{
    FTexture2DMipMap& mipmap = Img->PlatformData->Mips[0];
    unsigned char* Data = (unsigned char*)mipmap.BulkData.Lock(LOCK_READ_WRITE);
    int texturex = Img->PlatformData->SizeX;
    int texturey = Img->PlatformData->SizeY;
    TArray<FColor> nColors;
    float bPerUV_v = 1.0 / texturey;
    for (int32 y = 0; y < texturey; y++)
    {
        for (int32 x = 0; x < texturex; x++)
        {
            FColor bColor;
            bColor.B = Data[(y * texturex + x) * 4 + 0];//B
            bColor.G = Data[(y * texturex + x) * 4 + 1];//G
            bColor.R = Data[(y * texturex + x) * 4 + 2];//R
            bColor.A = Data[(y * texturex + x) * 4 + 3];//A 0:全透明；255:全不透明
            nColors.Add(bColor);
        }
    }
    //int32 strwide = (int32)(sizeof(uint8) * 4);
    //FMemory::Memcpy(Data, colors.GetData(), strwide*texturey*texturex);
    mipmap.BulkData.Unlock();
    Img->UpdateResource();

    //保存，思路，拿到Color的数据，然后利用FImageUtils转换图片数据，最后写入
    TArray<uint8> ImgData;
    FImageUtils::CompressImageArray(texturex, texturey, nColors, ImgData);
    bool re = FFileHelper::SaveArrayToFile(ImgData, *fileDestination);
    return re;
}

void UTextureLibrary::exportTextureAsImage(UTexture2D* texture, FString exportPath)
{
    // 直接在这里原地调用export接口
    ExprotUTexture2D(texture, exportPath);
}
