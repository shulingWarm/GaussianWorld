// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "MeshGenerator2.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API AMeshGenerator2 : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void makeDynamicMesh(UDynamicMesh* targetMesh);

	//��������ģ����ͼ������
	UFUNCTION(BlueprintCallable)
	UTexture2D* makeTestTexture();
};
