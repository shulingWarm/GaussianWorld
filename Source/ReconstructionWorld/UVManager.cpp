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
	//��սڵ�����ļ�¼
	this->vertexUVMap.clear();
}

void UVManager::registerUV(uint32 idVertex)
{
	//�ж�map�����Ƿ��Ѿ���������ڵ�
	if (this->vertexUVMap.count(idVertex) == 0)
	{
		this->vertexUVMap[idVertex] = this->totalUvId;
		++(this->totalUvId);
	}
}

//������UV�ڵ��ע��
void UVManager::endRegisterUV(FMeshShapeGenerator& generator)
{
	//��generator��չλ��
	generator.ExtendBufferSizes(0, 0, this->vertexUVMap.size(),
		this->vertexUVMap.size());
	//������˵��չ��֮��Ӧ�ú�total�ĸ�����һ����
	if (this->totalUvId != generator.UVs.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("this->totalUvId != generator.UVs.Num()"));
	}
	//��չ��λ��֮�󣬸�ÿ��UV��normal�����������Ľڵ�
	for (auto& eachPair : this->vertexUVMap)
	{
		generator.UVParentVertex[eachPair.second] = eachPair.first;
		generator.NormalParentVertex[eachPair.second] = eachPair.first;
	}
}

void UVManager::setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator)
{
	//������ǰ��¼������ÿ��UV
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

//���þ��ε���Ⱦ����
	//�����������Ҫ����UVһ����4��
	//һ�����������������Ⱦ�����ʱ����������Ĳ���
void UVManager::setRectangleUV(
	//������λ�ھ����ĸ��ǵ�UV����
	//���������洫�����ԭʼ��vertex id
	//������Ҫ�Լ������ĳ�uv id��
	uint32 id00,
	uint32 id10,//����x=1, y=0
	uint32 id01,
	uint32 id11,
	FMeshShapeGenerator& generator
)
{
	//��������ÿ����Ӧλ�õ�uv id
	generator.UVs[this->getUV(id00)] = { 0,0 };
	generator.UVs[this->getUV(id10)] = { 1,0 };
	generator.UVs[this->getUV(id01)] = { 0,1 };
	generator.UVs[this->getUV(id11)] = { 1,1 };
}