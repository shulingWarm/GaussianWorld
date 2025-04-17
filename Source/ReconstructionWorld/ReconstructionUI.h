// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "CoreMinimal.h"
#include"GaussianDescriptor.h"
#include"GaussianStruct.h"
#include "GameFramework/Actor.h"
#include "ReconstructionUI.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API AReconstructionUI : public AActor
{
	GENERATED_BODY()
	
public:	
	//用于准备函数的module
	typedef void (*ReconstructionFunc)(const char*,const char*,void*);
	//用于重建的函数指针
	ReconstructionFunc reconstruction_func;

	// Sets default values for this actor's properties
	AReconstructionUI();

	//执行重建过程
	UFUNCTION(BlueprintCallable)
	void beginReconstruction();

	//加载ply文件
	UFUNCTION(BlueprintCallable)
	void cppLoadPlyFile(FString plyPath, UGaussianDescriptor* descriptor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
