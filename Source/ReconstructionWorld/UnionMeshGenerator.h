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
	//���е�mesh��Ϣ
	std::vector<UDynamicMesh*> allMeshData;

	// Sets default values for this actor's properties
	AUnionMeshGenerator();

	//��������
	UFUNCTION(BlueprintCallable)
	void generateTable();

	//ע��һ���µ�mesh
	UFUNCTION(BlueprintCallable)
	void registerDynamicMesh(UDynamicMesh* mesh);

	//����һ����̬��mesh
	UFUNCTION(BlueprintImplementableEvent)
	void requestMeshComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
