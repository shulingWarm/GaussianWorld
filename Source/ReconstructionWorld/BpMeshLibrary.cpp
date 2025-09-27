// Fill out your copyright notice in the Description page of Project Settings.


#include "BpMeshLibrary.h"

UTexture2D* UBpMeshLibrary::buildUeMeshFromSolver(UDynamicMesh* mesh, UMeshDescriptor* meshDesc)
{
	// ��mesh solver����mesh
	meshDesc->meshSolver->buildUeMesh(mesh);
	return meshDesc->meshSolver->makeUeTexture();
}
