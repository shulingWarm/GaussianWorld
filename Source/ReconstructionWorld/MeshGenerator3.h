// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "MeshGenerator3.generated.h"

/**
 * 
 */
UCLASS()
class RECONSTRUCTIONWORLD_API AMeshGenerator3 : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void makeDynamicMesh(UDynamicMesh* targetMesh);

	//��������Բ����ķ���
	UFUNCTION(BlueprintCallable)
	void makeCylinder(UDynamicMesh* targetMesh);

	//���ɻ��ε�mesh
	UFUNCTION(BlueprintCallable)
	void makeArchMesh(UDynamicMesh* targetMesh);

	//���meshԭ��
	static void addMeshPrimitive(UDynamicMesh* targetMesh,
		UE::Geometry::FMeshShapeGenerator* generator,
		FTransform transform,
		FVector3d preTranslate
	);
};
