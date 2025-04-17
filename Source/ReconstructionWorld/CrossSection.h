// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include<vector>
#include "GameFramework/Actor.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API CrossSection
{
public:
	CrossSection();
	~CrossSection();

	//�ڵ��б�
	std::vector<FVector> vertexList;
	//��һ������ �ⶫ�����������UV�����
	std::vector<FVector2f> normalVertex;

	//��ӽڵ�
	void addVertex(FVector vertex);

	//����section����Ĺ�һ������
	void generateNormalCoord();
};
