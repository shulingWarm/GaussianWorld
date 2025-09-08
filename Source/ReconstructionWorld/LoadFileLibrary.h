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

	//��ȡ�����ؽ���ͼƬ·��
	UFUNCTION(BlueprintCallable)
	static FString getImageFolder();

	//����ָ���ļ��ĺ���
	UFUNCTION(BlueprintCallable)
	static FString getSpecificFile(FString extension);

	// ��ȡ�ļ����������е��ļ�
	static void GetFilesInDirectory(const FString& Directory, TArray<FString>& FileNames);

	// �����������鱣����ļ�
	static void saveByteArrayAsFile(const char* saveData, uint32_t dataSize, std::string filePath);

	// ��ȡ�ļ��ĺ�׺��
	static std::string getFileSuffix(std::string filePath);
};
