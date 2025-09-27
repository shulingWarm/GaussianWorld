// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"MeshSolver.hpp"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include"Types.hpp"
#include "MeshDescriptor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RECONSTRUCTIONWORLD_API UMeshDescriptor : public UObject
{
	GENERATED_BODY()
public:

	UMeshDescriptor();

	void initMeshSolver(Ptr<MeshSolver> newMeshSolver);

	Ptr<MeshSolver> meshSolver = nullptr;
};
