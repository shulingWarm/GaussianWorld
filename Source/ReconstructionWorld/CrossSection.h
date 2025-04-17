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

	//节点列表
	std::vector<FVector> vertexList;
	//归一化坐标 这东西是用来添加UV坐标的
	std::vector<FVector2f> normalVertex;

	//添加节点
	void addVertex(FVector vertex);

	//生成section里面的归一化坐标
	void generateNormalCoord();
};
