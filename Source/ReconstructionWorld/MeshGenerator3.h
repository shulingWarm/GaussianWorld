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

	//处理生成圆柱体的方法
	UFUNCTION(BlueprintCallable)
	void makeCylinder(UDynamicMesh* targetMesh);

	//生成弧形的mesh
	UFUNCTION(BlueprintCallable)
	void makeArchMesh(UDynamicMesh* targetMesh);

	//添加mesh原语
	static void addMeshPrimitive(UDynamicMesh* targetMesh,
		UE::Geometry::FMeshShapeGenerator* generator,
		FTransform transform,
		FVector3d preTranslate
	);
};
