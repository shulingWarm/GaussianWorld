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
	//构造dynamic mesh
	requestMeshComponent();
	//如果申请mesh失败了就直接退出
	if (allMeshData.size() == 0) {
		return;
	}
	
	//生成桌板部分的代码
	{
		//获取新添加的mesh
		//新的mesh作为桌板的部分
		auto boardMesh = allMeshData.back();
		CrossSection roundRectSection;
		SectionGenerator::roundRectangle(roundRectSection, 100, 50, 8);
		//根据section生成mesh
		MeshGeneratorBySection meshGenerator(roundRectSection, 6);
		//生成一般的矩形mesh
		meshGenerator.Generate();
		//给mesh添加原语
		MeshLibrary::addMeshPrimitive(boardMesh, &meshGenerator, FTransform(),
			FVector3d::Zero());
	}
	//生成支撑杆
	{
		//申请新的动态mesh
		requestMeshComponent();
		//获取新生成的动态mesh
		auto supportMesh = allMeshData.back();
		//生成圆形横截面
		CrossSection circleSection;
		SectionGenerator::circle(circleSection, 8);
		//根据section生成弧形mesh
		MeshGeneratorBySection meshGenerator(circleSection, 40);
		//把mesh generator调整成弧形的生成mesh逻辑
		meshGenerator.generateMethod = MeshGeneratorBySection::RECT_ARCH;
		//执行mesh生成
		meshGenerator.Generate();
		//给mesh添加原语
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

