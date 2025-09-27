// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshDescriptor.h"

UMeshDescriptor::UMeshDescriptor()
{
}

void UMeshDescriptor::initMeshSolver(Ptr<MeshSolver> newMeshSolver)
{
	this->meshSolver = newMeshSolver;
}
