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
	//Բ�Ǿ��εķ�Χ
	float rectRange[4] = { 0, 100, 0, 50 };
	//����Բ�Ǿ��εĹ���
	CrossSection section;
	RoundRectangleGenerator::makeRoundRectangleSection(
		section, rectRange
	);
	//�½����ݺ��������mesh��ʵ��
	MeshGeneratorBySection meshGenerator(section);
	//Ȼ��������������generate����
	meshGenerator.Generate();
	//��������Ҫʵ�����meshԭ��Ĺ���
	addMeshPrimitive(targetMesh, &meshGenerator, FTransform(), FVector3d::Zero());
}

void AMeshGenerator3::makeCylinder(UDynamicMesh* targetMesh)
{
	//����Բ�εĽ���
	CrossSection section;
	SectionGenerator::circle(section, 10);
	//����section����mesh
	MeshGeneratorBySection meshGenerator(section, 200);
	//ִ��generate����
	meshGenerator.Generate();
	//���ԭ�����������ӵ�mesh��
	addMeshPrimitive(targetMesh, &meshGenerator, FTransform(), FVector3d::Zero());
}

void AMeshGenerator3::makeArchMesh(UDynamicMesh* targetMesh)
{
	//����Բ�εĽ���
	CrossSection section;
	SectionGenerator::circle(section, 10);
	//����section����mesh
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


