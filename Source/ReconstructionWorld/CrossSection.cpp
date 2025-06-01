// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossSection.h"

CrossSection::CrossSection()
{
}

CrossSection::~CrossSection()
{
}

void CrossSection::addVertex(FVector vertex)
{
	this->vertexList.push_back(vertex);
}

void CrossSection::addVertex(float x, float y)
{
	this->addVertex(FVector(x, y, 0));
}

//基本的更新坐标范围的方法
void updateRange(float* range, float value)
{
	if (value < range[0])
		range[0] = value;
	else if (value > range[1])
		range[1] = value;
}

//计算一个数值在范围内的归一化位置
float getNormalLocation(float* range, float value)
{
	return (value - range[0]) / (range[1] - range[0] + 1e-9);
}

void CrossSection::generateNormalCoord()
{
	//如果坐标列表是空的就可以直接结束了
	if (this->vertexList.size() == 0)
		return;
	//重置normal vertex的个数
	this->normalVertex.resize(this->vertexList.size());
	//计算所有坐标的范围
	float range[4];
	range[0] = vertexList[0].X;
	range[1] = vertexList[0].X;
	range[2] = vertexList[0].Y;
	range[3] = vertexList[0].Y;
	//遍历所有的点更新范围
	for (uint32 id = 1; id < vertexList.size(); ++id)
	{
		//当前位置的节点坐标
		auto& currVertex = this->vertexList[id];
		//当前位置的范围更新
		updateRange(range, currVertex.X);
		updateRange(range + 2, currVertex.Y);
	}
	//遍历每个点，更新它的归一化坐标
	for (uint32 id = 0; id < vertexList.size(); ++id)
	{
		//当前位置的节点
		auto& oriVertex = this->vertexList[id];
		//当前位置的归一化坐标
		auto& oneVertex = this->normalVertex[id];
		oneVertex.X = getNormalLocation(range, oriVertex.X);
		oneVertex.Y = getNormalLocation(range, oriVertex.Y);
	}
}
