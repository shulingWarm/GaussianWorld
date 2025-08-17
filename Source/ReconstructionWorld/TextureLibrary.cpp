// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureLibrary.h"
#include"TextureConfigLibrary.h"
#include"ImageUtils.h"
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
            bColor.A = Data[(y * texturex + x) * 4 + 3];//A 0:ȫ͸����255:ȫ��͸��
            nColors.Add(bColor);
        }
    }
    //int32 strwide = (int32)(sizeof(uint8) * 4);
    //FMemory::Memcpy(Data, colors.GetData(), strwide*texturey*texturex);
    mipmap.BulkData.Unlock();
    Img->UpdateResource();

    //���棬˼·���õ�Color�����ݣ�Ȼ������FImageUtilsת��ͼƬ���ݣ����д��
    TArray<uint8> ImgData;
    FImageUtils::CompressImageArray(texturex, texturey, nColors, ImgData);
    bool re = FFileHelper::SaveArrayToFile(ImgData, *fileDestination);
    return re;
}

void UTextureLibrary::exportTextureAsImage(UTexture2D* texture, FString exportPath)
{
    // ֱ��������ԭ�ص���export�ӿ�
    ExprotUTexture2D(texture, exportPath);
}
