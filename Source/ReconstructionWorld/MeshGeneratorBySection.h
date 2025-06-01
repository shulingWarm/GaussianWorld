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
	//����section����Mesh�ķ���
	enum GenerateMethod {
		CYLINDER, //�򵥵�Բ����
		RECT_ARCH //ֱ�ǻ���
	};

	uint32 faceNum = 0;
	//�ڲ���UV������
	UVManager uvManager;
	//����sectionʱ��mesh�߶�
	float meshHeight = 0;
	//Ŀǰѡ�õ�mesh���ɷ���
	GenerateMethod generateMethod = GenerateMethod::CYLINDER;

	MeshGeneratorBySection();
	~MeshGeneratorBySection();

	//��������������section��������Generate
	CrossSection* sectionPtr;

	//���mesh height�Ǹ����ɼ�mesh�����ʹ�õ�
	MeshGeneratorBySection(CrossSection& section, float meshHeight=7);

	//��ָ����Zλ����ӽ���
	void addSection(CrossSection& section, float z);

	//����transform���section
	void addSection(CrossSection& section, FTransform3f& transform);

	//�������ĵ��ӳ���ϵ���section
	//idMap��ʾ����XY��λ�ö�Ӧ��XYZ�����λ��
	void addSection(CrossSection& section, FVector3f center, FVector2i idMap);

	//����section����mesh�Ĺ���
	void makeMeshBySection();

	//����section���ɻ��ε�����mesh
	void makeArchMesh();

	//�պ�section
	void closeSection(uint32 idSection, CrossSection& section, bool clockFlag);

	//ע�Ტ����µ�face id
	uint32 getNewFaceId();

	//����section���߼�
	void linkSection(CrossSection& section,
		int idSection //�������section��������һ��section
	);

	//����һ�������������
	void addTriangleOnce(
		uint32 idTriangle, uint32 id0, uint32 id1, uint32 id2, uint32 faceId
	);

	virtual FMeshShapeGenerator& Generate() override;
};
