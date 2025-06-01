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

//�����ĸ������귶Χ�ķ���
void updateRange(float* range, float value)
{
	if (value < range[0])
		range[0] = value;
	else if (value > range[1])
		range[1] = value;
}

//����һ����ֵ�ڷ�Χ�ڵĹ�һ��λ��
float getNormalLocation(float* range, float value)
{
	return (value - range[0]) / (range[1] - range[0] + 1e-9);
}

void CrossSection::generateNormalCoord()
{
	//��������б��ǿյľͿ���ֱ�ӽ�����
	if (this->vertexList.size() == 0)
		return;
	//����normal vertex�ĸ���
	this->normalVertex.resize(this->vertexList.size());
	//������������ķ�Χ
	float range[4];
	range[0] = vertexList[0].X;
	range[1] = vertexList[0].X;
	range[2] = vertexList[0].Y;
	range[3] = vertexList[0].Y;
	//�������еĵ���·�Χ
	for (uint32 id = 1; id < vertexList.size(); ++id)
	{
		//��ǰλ�õĽڵ�����
		auto& currVertex = this->vertexList[id];
		//��ǰλ�õķ�Χ����
		updateRange(range, currVertex.X);
		updateRange(range + 2, currVertex.Y);
	}
	//����ÿ���㣬�������Ĺ�һ������
	for (uint32 id = 0; id < vertexList.size(); ++id)
	{
		//��ǰλ�õĽڵ�
		auto& oriVertex = this->vertexList[id];
		//��ǰλ�õĹ�һ������
		auto& oneVertex = this->normalVertex[id];
		oneVertex.X = getNormalLocation(range, oriVertex.X);
		oneVertex.Y = getNormalLocation(range, oriVertex.Y);
	}
}
