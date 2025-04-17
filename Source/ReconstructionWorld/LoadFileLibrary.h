// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoadFileLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API ULoadFileLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

	//获取用于重建的图片路径
	UFUNCTION(BlueprintCallable)
	static FString getImageFolder();

	//加载指定文件的函数
	UFUNCTION(BlueprintCallable)
	static FString getSpecificFile(FString extension);

};
