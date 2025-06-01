// Fill out your copyright notice in the Description page of Project Settings.


#include "SectionGenerator.h"
#include "RoundRectangleGenerator.h"

SectionGenerator::SectionGenerator()
{

}

SectionGenerator::~SectionGenerator()
{

}

void SectionGenerator::circle(CrossSection& section, float radius)
{
	//������������͹��Ұ�16�������
	constexpr uint32 sampleNum = 16;
	//ÿ���������Ļ���
	float stepRad = 2 * PI / sampleNum;
	//������������ӵ�
	for (uint32 i = 0; i < sampleNum; ++i) {
		//���㵱ǰλ�õ�xy
		float x = radius * std::cos(stepRad * i);
		float y = radius * std::sin(stepRad * i);
		//��xy������ӽ�������
		section.addVertex(x, y);
	}
	section.generateNormalCoord();
}

void SectionGenerator::roundRectangle(CrossSection& section, float xLength, float yLength, float radius)
{
	//Բ�Ǿ��εķ�Χ
	float range[4] = { -xLength / 2, xLength / 2, -yLength / 2, yLength / 2 };
	//Բ�ǵİ뾶����
	float roundRate = radius * 2 / std::min(xLength, yLength);
	//����Բ�Ǿ��εĽ���
	RoundRectangleGenerator::makeRoundRectangleSection(section,
		range, roundRate);
}
