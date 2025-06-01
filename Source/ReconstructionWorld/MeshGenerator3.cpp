// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator3.h"
#include "CrossSection.h"
#include "RoundRectangleGenerator.h"
#include "MeshGeneratorBySection.h"
#include "SectionGenerator.h"
#include "DynamicMeshEditor.h"
#include "DynamicMesh/MeshTransforms.h"

void AMeshGenerator3::makeDynamicMesh(UDynamicMesh* targetMesh)
{
	//圆角矩形的范围
	float rectRange[4] = { 0, 100, 0, 50 };
	//生成圆角矩形的工具
	CrossSection section;
	RoundRectangleGenerator::makeRoundRectangleSection(
		section, rectRange
	);
	//新建根据横截面生成mesh的实体
	MeshGeneratorBySection meshGenerator(section);
	//然后调用这个东西的generate过程
	meshGenerator.Generate();
	//在下面需要实现添加mesh原语的过程
	addMeshPrimitive(targetMesh, &meshGenerator, FTransform(), FVector3d::Zero());
}

void AMeshGenerator3::makeCylinder(UDynamicMesh* targetMesh)
{
	//生成圆形的截面
	CrossSection section;
	SectionGenerator::circle(section, 10);
	//根据section生成mesh
	MeshGeneratorBySection meshGenerator(section, 200);
	//执行generate操作
	meshGenerator.Generate();
	//添加原语，把这玩意添加到mesh中
	addMeshPrimitive(targetMesh, &meshGenerator, FTransform(), FVector3d::Zero());
}

void AMeshGenerator3::makeArchMesh(UDynamicMesh* targetMesh)
{
	//生成圆形的截面
	CrossSection section;
	SectionGenerator::circle(section, 10);
	//根据section生成mesh
	MeshGeneratorBySection meshGenerator(section, 200);
	meshGenerator.generateMethod = MeshGeneratorBySection::RECT_ARCH;
	meshGenerator.Generate();
	addMeshPrimitive(targetMesh, &meshGenerator, FTransform(), FVector3d::Zero());
}

void AMeshGenerator3::addMeshPrimitive(UDynamicMesh* targetMesh, FMeshShapeGenerator* generator, FTransform transform, FVector3d preTranslate)
{
	auto ApplyOptionsToMesh = [&transform, preTranslate](FDynamicMesh3& Mesh)
		{
			if (preTranslate.SquaredLength() > 0)
			{
				MeshTransforms::Translate(Mesh, preTranslate);
			}

			MeshTransforms::ApplyTransform(Mesh, (FTransformSRT3d)transform, true);
		};

	if (targetMesh->IsEmpty())
	{
		targetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
			{
				EditMesh.Copy(generator);
				ApplyOptionsToMesh(EditMesh);
			}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	}
	else
	{
		FDynamicMesh3 TempMesh(generator);
		ApplyOptionsToMesh(TempMesh);
		targetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
			{
				FMeshIndexMappings TmpMappings;
				FDynamicMeshEditor Editor(&EditMesh);
				Editor.AppendMesh(&TempMesh, TmpMappings);

			}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	}
}


