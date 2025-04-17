// Fill out your copyright notice in the Description page of Project Settings.


#include "GaussianDescriptor.h"

// Sets default values for this component's properties
UGaussianDescriptor::UGaussianDescriptor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UTexture2D* UGaussianDescriptor::getPosTexture()
{
	return this->textreList[0];
}

UTexture2D* UGaussianDescriptor::getColorTexture()
{
	return this->textreList[1];
}

UTexture2D* UGaussianDescriptor::getScaleTexture()
{
	return this->textreList[2];
}

UTexture2D* UGaussianDescriptor::getRotTexture()
{
	return this->textreList[3];
}


// Called when the game starts
void UGaussianDescriptor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGaussianDescriptor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

