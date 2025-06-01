// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CrossSection.h"
#include "Generators/MeshShapeGenerator.h"
#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API RoundRectangleGenerator
{
public:
	RoundRectangleGenerator();
	~RoundRectangleGenerator();

	//����Բ�Ǿ��εĽ���
	static void makeRoundRectangleSection(CrossSection& dstSection, 
		float* rectRange,
		float roundRate = 0.25f);
};
