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
	

	//��ȡ�����ؽ���ͼƬ·��
	UFUNCTION(BlueprintCallable)
	static FString getImageFolder();

	//����ָ���ļ��ĺ���
	UFUNCTION(BlueprintCallable)
	static FString getSpecificFile(FString extension);

};
