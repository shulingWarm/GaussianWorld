// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Generators/MeshShapeGenerator.h"
#include"UDynamicMesh.h"
#include "MeshDescriptor.h"
#include "BpMeshLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API UBpMeshLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UTexture2D* buildUeMeshFromSolver(UDynamicMesh* mesh, UMeshDescriptor* meshDesc);
};
