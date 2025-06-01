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
	//采样点个数，就姑且按16个来算吧
	constexpr uint32 sampleNum = 16;
	//每个采样步的弧度
	float stepRad = 2 * PI / sampleNum;
	//依次向里面添加点
	for (uint32 i = 0; i < sampleNum; ++i) {
		//计算当前位置的xy
		float x = radius * std::cos(stepRad * i);
		float y = radius * std::sin(stepRad * i);
		//把xy坐标添加进截面中
		section.addVertex(x, y);
	}
	section.generateNormalCoord();
}

void SectionGenerator::roundRectangle(CrossSection& section, float xLength, float yLength, float radius)
{
	//圆角矩形的范围
	float range[4] = { -xLength / 2, xLength / 2, -yLength / 2, yLength / 2 };
	//圆角的半径比例
	float roundRate = radius * 2 / std::min(xLength, yLength);
	//生成圆角矩形的截面
	RoundRectangleGenerator::makeRoundRectangleSection(section,
		range, roundRate);
}
