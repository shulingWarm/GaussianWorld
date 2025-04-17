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
	//���سɹ���texture�б�
	TArray<UTexture2D*> textreList;


	//3D��˹�ĵ����
	UPROPERTY(BlueprintReadWrite)
	int gaussianNum;

	//XY�Ĳ�����С
	UPROPERTY(BlueprintReadWrite)
	int sampleSizeX;
	UPROPERTY(BlueprintReadWrite)
	int sampleSizeY;

	//��������
	UPROPERTY(BlueprintReadWrite)
	FVector2D sampleStep;

	// Sets default values for this component's properties
	UGaussianDescriptor();

	//���λ��Texture
	UFUNCTION(BlueprintCallable)
	UTexture2D* getPosTexture();

	//��ȡ��ɫTexture
	UFUNCTION(BlueprintCallable)
	UTexture2D* getColorTexture();

	//��ȡScale Texture
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
