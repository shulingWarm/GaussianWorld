// Fill out your copyright notice in the Description page of Project Settings.


#include "UVManager.h"

UVManager::UVManager()
{
}

UVManager::~UVManager()
{
}

void UVManager::beginRegisterUV()
{
	//清空节点里面的记录
	this->vertexUVMap.clear();
}

void UVManager::registerUV(uint32 idVertex)
{
	//判断map里面是否已经有了这个节点
	if (this->vertexUVMap.count(idVertex) == 0)
	{
		this->vertexUVMap[idVertex] = this->totalUvId;
		++(this->totalUvId);
	}
}

//结束对UV节点的注册
void UVManager::endRegisterUV(FMeshShapeGenerator& generator)
{
	//给generator扩展位置
	generator.ExtendBufferSizes(0, 0, this->vertexUVMap.size(),
		this->vertexUVMap.size());
	//正常来说扩展完之后应该和total的个数是一样的
	if (this->totalUvId != generator.UVs.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("this->totalUvId != generator.UVs.Num()"));
	}
	//扩展好位置之后，给每个UV和normal绑定它们所属的节点
	for (auto& eachPair : this->vertexUVMap)
	{
		generator.UVParentVertex[eachPair.second] = eachPair.first;
		generator.NormalParentVertex[eachPair.second] = eachPair.first;
	}
}

void UVManager::setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator)
{
	//遍历当前记录下来的每个UV
	for (auto& eachPair : this->vertexUVMap)
	{
		generator.Normals[eachPair.second] = normal;
	}
}

uint32 UVManager::getUV(uint32 idVertex)
{
	if (this->vertexUVMap.count(idVertex) == 0)
		return 0;
	return this->vertexUVMap.at(idVertex);
}

//设置矩形的渲染坐标
	//这种情况下需要假设UV一共就4个
	//一般在做横截面连接渲染侧面的时候会有这样的操作
void UVManager::setRectangleUV(
	//下面是位于矩形四个角的UV坐标
	//而且这里面传入的是原始的vertex id
	//这是需要自己把它改成uv id的
	uint32 id00,
	uint32 id10,//这是x=1, y=0
	uint32 id01,
	uint32 id11,
	FMeshShapeGenerator& generator
)
{
	//依次设置每个对应位置的uv id
	generator.UVs[this->getUV(id00)] = { 0,0 };
	generator.UVs[this->getUV(id10)] = { 1,0 };
	generator.UVs[this->getUV(id01)] = { 0,1 };
	generator.UVs[this->getUV(id11)] = { 1,1 };
}