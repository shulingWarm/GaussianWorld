// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<unordered_map>
#include "Generators/MeshShapeGenerator.h"
#include "CoreMinimal.h"

using namespace UE::Geometry;

/**
 * 
 */
class RECONSTRUCTIONWORLD_API UVManager
{
public:
	//目前已经分配过的总的UV的个数
	uint32 totalUvId = 0;
	//当前状态下的每个节点id对应的UV映射表
	std::unordered_map<uint32, uint32> vertexUVMap;

	UVManager();
	~UVManager();

	//标记开始注册UV
	void beginRegisterUV();

	//注册节点
	void registerUV(uint32 idVertex);

	//结束对UV节点的注册
	void endRegisterUV(FMeshShapeGenerator& generator);

	//给当前注册的每个UV设置normal
	//假设同一个面上的所有点的法向量都是一样的
	void setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator);

	//设置矩形的渲染坐标
	//这种情况下需要假设UV一共就4个
	//一般在做横截面连接渲染侧面的时候会有这样的操作
	void setRectangleUV(
		//下面是位于矩形四个角的UV坐标
		//而且这里面传入的是原始的vertex id
		//这是需要自己把它改成uv id的
		uint32 id00,
		uint32 id10,//这是x=1, y=0
		uint32 id01,
		uint32 id11,
		FMeshShapeGenerator& generator
	);

	//获得节点对应的UV
	uint32 getUV(uint32 idVertex);
};
