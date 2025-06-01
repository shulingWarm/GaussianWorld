// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UVManager.h"
#include "CrossSection.h"
#include "Generators/MeshShapeGenerator.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API MeshGeneratorBySection : public UE::Geometry::FMeshShapeGenerator
{
public:
	//根据section生成Mesh的方法
	enum GenerateMethod {
		CYLINDER, //简单的圆柱形
		RECT_ARCH //直角弧形
	};

	uint32 faceNum = 0;
	//内部的UV管理器
	UVManager uvManager;
	//生成section时的mesh高度
	float meshHeight = 0;
	//目前选用的mesh生成方法
	GenerateMethod generateMethod = GenerateMethod::CYLINDER;

	MeshGeneratorBySection();
	~MeshGeneratorBySection();

	//保存在生成器的section，用于在Generate
	CrossSection* sectionPtr;

	//这个mesh height是给生成简单mesh的情况使用的
	MeshGeneratorBySection(CrossSection& section, float meshHeight=7);

	//在指定的Z位置添加截面
	void addSection(CrossSection& section, float z);

	//根据transform添加section
	void addSection(CrossSection& section, FTransform3f& transform);

	//根据中心点和映射关系添加section
	//idMap表示的是XY的位置对应到XYZ里面的位置
	void addSection(CrossSection& section, FVector3f center, FVector2i idMap);

	//根据section生成mesh的过程
	void makeMeshBySection();

	//根据section生成弧形的曲线mesh
	void makeArchMesh();

	//闭合section
	void closeSection(uint32 idSection, CrossSection& section, bool clockFlag);

	//注册并获得新的face id
	uint32 getNewFaceId();

	//连接section的逻辑
	void linkSection(CrossSection& section,
		int idSection //连接这个section和它的上一个section
	);

	//集成一次性添加三角形
	void addTriangleOnce(
		uint32 idTriangle, uint32 id0, uint32 id1, uint32 id2, uint32 faceId
	);

	virtual FMeshShapeGenerator& Generate() override;
};
