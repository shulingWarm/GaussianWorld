// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"CrossSection.h"
#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API SectionGenerator
{
public:
	SectionGenerator();
	~SectionGenerator();

	//圆形的截面生成器
	static void circle(CrossSection& section, float radius);

	//圆角矩形
	static void roundRectangle(CrossSection& section,
		float xLength, float yLength, float radius
	);

};
