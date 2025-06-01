// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshLibrary.h"
#include"DynamicMesh/MeshTransforms.h"
#include"DynamicMeshEditor.h"

using namespace UE::Geometry;

MeshLibrary::MeshLibrary()
{

}

MeshLibrary::~MeshLibrary()
{

}

void MeshLibrary::addMeshPrimitive(UDynamicMesh* targetMesh, UE::Geometry::FMeshShapeGenerator* generator, FTransform transform, FVector3d preTranslate)
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
