// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UDynamicMesh.h"
#include "Generators/MeshShapeGenerator.h"
#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API MeshLibrary
{
public:
	MeshLibrary();
	~MeshLibrary();

	//给mesh添加generator的原语
	static void addMeshPrimitive(UDynamicMesh* targetMesh,
		UE::Geometry::FMeshShapeGenerator* generator,
		FTransform transform,
		FVector3d preTranslate
	);
};
