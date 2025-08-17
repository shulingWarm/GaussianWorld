// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API UTextureLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	//生成木纹贴图
	UFUNCTION(BlueprintCallable)
	static UTexture2D* makeWoodTexture();

	// 把texture保存成图片
	UFUNCTION(BlueprintCallable)
	static void exportTextureAsImage(UTexture2D* texture, FString exportPath);
};
