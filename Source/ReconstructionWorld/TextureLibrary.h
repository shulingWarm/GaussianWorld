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
	
	//Éú³ÉÄ¾ÎÆÌùÍ¼
	UFUNCTION(BlueprintCallable)
	static UTexture2D* makeWoodTexture();
};
