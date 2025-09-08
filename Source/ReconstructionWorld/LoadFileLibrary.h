// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include<string>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoadFileLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API ULoadFileLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//获取用于重建的图片路径
	UFUNCTION(BlueprintCallable)
	static FString getImageFolder();

	//加载指定文件的函数
	UFUNCTION(BlueprintCallable)
	static FString getSpecificFile(FString extension);

	// 获取文件夹里面所有的文件
	static void GetFilesInDirectory(const FString& Directory, TArray<FString>& FileNames);

	// 将二进制数组保存成文件
	static void saveByteArrayAsFile(const char* saveData, uint32_t dataSize, std::string filePath);

	// 获取文件的后缀名
	static std::string getFileSuffix(std::string filePath);
};
