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

	//Բ�εĽ���������
	static void circle(CrossSection& section, float radius);

	//Բ�Ǿ���
	static void roundRectangle(CrossSection& section,
		float xLength, float yLength, float radius
	);

};
