// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppRotationAxis.generated.h"

UCLASS()
class RECONSTRUCTIONWORLD_API ACppRotationAxis : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACppRotationAxis();

	//������������Ļ�ϵķ���
	UFUNCTION(BlueprintCallable)
	FVector2D GetTangentVectorInScreen(APlayerController* playerController, FVector worldDirection, FVector axisCenter, FVector planeNorm,
		FVector cameraCenter, FRotator cameraRotation);

	//��������һЩ�м��������
	UFUNCTION(BlueprintCallable)
	TArray<FVector> TangentDebug(FVector worldDirection, FVector axisCenter, FVector planeNorm,
		FVector cameraCenter, FRotator cameraRotation);

	//����ͶӰ�߷���
	//��ֻ�ǰ�ͶӰ�ߴ�ӡ�������У�Ȼ������Ҹ�matlab����֤�������
	UFUNCTION(BlueprintCallable)
	FVector printTangentLine(FVector worldDirection, FVector axisCenter, FVector planeNorm,
		FVector cameraCenter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
