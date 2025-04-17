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

	//计算切线在屏幕上的方向
	UFUNCTION(BlueprintCallable)
	FVector2D GetTangentVectorInScreen(APlayerController* playerController, FVector worldDirection, FVector axisCenter, FVector planeNorm,
		FVector cameraCenter, FRotator cameraRotation);

	//用来测试一些中间向量结果
	UFUNCTION(BlueprintCallable)
	TArray<FVector> TangentDebug(FVector worldDirection, FVector axisCenter, FVector planeNorm,
		FVector cameraCenter, FRotator cameraRotation);

	//计算投影线方程
	//这只是把投影线打印出来就行，然后随便找个matlab来验证这个东西
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
