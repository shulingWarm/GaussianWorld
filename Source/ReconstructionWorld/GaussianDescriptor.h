// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GaussianDescriptor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RECONSTRUCTIONWORLD_API UGaussianDescriptor : public UActorComponent
{
	GENERATED_BODY()

public:	
	//加载成功的texture列表
	TArray<UTexture2D*> textreList;


	//3D高斯的点个数
	UPROPERTY(BlueprintReadWrite)
	int gaussianNum;

	//XY的采样大小
	UPROPERTY(BlueprintReadWrite)
	int sampleSizeX;
	UPROPERTY(BlueprintReadWrite)
	int sampleSizeY;

	//采样步长
	UPROPERTY(BlueprintReadWrite)
	FVector2D sampleStep;

	// Sets default values for this component's properties
	UGaussianDescriptor();

	//获得位置Texture
	UFUNCTION(BlueprintCallable)
	UTexture2D* getPosTexture();

	//获取颜色Texture
	UFUNCTION(BlueprintCallable)
	UTexture2D* getColorTexture();

	//获取Scale Texture
	UFUNCTION(BlueprintCallable)
	UTexture2D* getScaleTexture();

	UFUNCTION(BlueprintCallable)
	UTexture2D* getRotTexture();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
