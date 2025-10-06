// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TransformLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API UTransformLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FTransform getCameraTransform(FVector location, float pitch, float planeAngle, float distance);
	
	UFUNCTION(BlueprintCallable)
	static FVector getCameraLocation(FVector location, float pitch, float planeAngle, float distance);
};
