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
	//����׼��������module
	typedef void (*ReconstructionFunc)(const char*,const char*,void*);
	//�����ؽ��ĺ���ָ��
	ReconstructionFunc reconstruction_func;

	// Sets default values for this actor's properties
	AReconstructionUI();

	//ִ���ؽ�����
	UFUNCTION(BlueprintCallable)
	void beginReconstruction();

	//����ply�ļ�
	UFUNCTION(BlueprintCallable)
	void cppLoadPlyFile(FString plyPath, UGaussianDescriptor* descriptor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
