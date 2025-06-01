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
	//Ŀǰ�Ѿ���������ܵ�UV�ĸ���
	uint32 totalUvId = 0;
	//��ǰ״̬�µ�ÿ���ڵ�id��Ӧ��UVӳ���
	std::unordered_map<uint32, uint32> vertexUVMap;

	UVManager();
	~UVManager();

	//��ǿ�ʼע��UV
	void beginRegisterUV();

	//ע��ڵ�
	void registerUV(uint32 idVertex);

	//������UV�ڵ��ע��
	void endRegisterUV(FMeshShapeGenerator& generator);

	//����ǰע���ÿ��UV����normal
	//����ͬһ�����ϵ����е�ķ���������һ����
	void setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator);

	//���þ��ε���Ⱦ����
	//�����������Ҫ����UVһ����4��
	//һ�����������������Ⱦ�����ʱ����������Ĳ���
	void setRectangleUV(
		//������λ�ھ����ĸ��ǵ�UV����
		//���������洫�����ԭʼ��vertex id
		//������Ҫ�Լ������ĳ�uv id��
		uint32 id00,
		uint32 id10,//����x=1, y=0
		uint32 id01,
		uint32 id11,
		FMeshShapeGenerator& generator
	);

	//��ýڵ��Ӧ��UV
	uint32 getUV(uint32 idVertex);
};
