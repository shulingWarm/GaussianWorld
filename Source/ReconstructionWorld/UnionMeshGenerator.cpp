// Fill out your copyright notice in the Description page of Project Settings.


#include "UnionMeshGenerator.h"
#include"SectionGenerator.h"
#include"MeshLibrary.h"
#include"MeshGeneratorBySection.h"

// Sets default values
AUnionMeshGenerator::AUnionMeshGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUnionMeshGenerator::generateTable()
{
	//����dynamic mesh
	requestMeshComponent();
	//�������meshʧ���˾�ֱ���˳�
	if (allMeshData.size() == 0) {
		return;
	}
	
	//�������岿�ֵĴ���
	{
		//��ȡ����ӵ�mesh
		//�µ�mesh��Ϊ����Ĳ���
		auto boardMesh = allMeshData.back();
		CrossSection roundRectSection;
		SectionGenerator::roundRectangle(roundRectSection, 100, 50, 8);
		//����section����mesh
		MeshGeneratorBySection meshGenerator(roundRectSection, 6);
		//����һ��ľ���mesh
		meshGenerator.Generate();
		//��mesh���ԭ��
		MeshLibrary::addMeshPrimitive(boardMesh, &meshGenerator, FTransform(),
			FVector3d::Zero());
	}
	//����֧�Ÿ�
	{
		//�����µĶ�̬mesh
		requestMeshComponent();
		//��ȡ�����ɵĶ�̬mesh
		auto supportMesh = allMeshData.back();
		//����Բ�κ����
		CrossSection circleSection;
		SectionGenerator::circle(circleSection, 8);
		//����section���ɻ���mesh
		MeshGeneratorBySection meshGenerator(circleSection, 40);
		//��mesh generator�����ɻ��ε�����mesh�߼�
		meshGenerator.generateMethod = MeshGeneratorBySection::RECT_ARCH;
		//ִ��mesh����
		meshGenerator.Generate();
		//��mesh���ԭ��
		MeshLibrary::addMeshPrimitive(supportMesh, &meshGenerator, FTransform(),
			FVector3d::Zero());
	}
}

void AUnionMeshGenerator::registerDynamicMesh(UDynamicMesh* mesh)
{
	this->allMeshData.push_back(mesh);
}

// Called when the game starts or when spawned
void AUnionMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnionMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

