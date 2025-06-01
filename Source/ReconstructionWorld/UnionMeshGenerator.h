// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include<vector>
#include "UDynamicMesh.h"
#include "GameFramework/Actor.h"
#include "UnionMeshGenerator.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API AUnionMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	//所有的mesh信息
	std::vector<UDynamicMesh*> allMeshData;

	// Sets default values for this actor's properties
	AUnionMeshGenerator();

	//生成桌子
	UFUNCTION(BlueprintCallable)
	void generateTable();

	//注册一个新的mesh
	UFUNCTION(BlueprintCallable)
	void registerDynamicMesh(UDynamicMesh* mesh);

	//申请一个动态的mesh
	UFUNCTION(BlueprintImplementableEvent)
	void requestMeshComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
