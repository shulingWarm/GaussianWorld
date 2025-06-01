// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGeneratorBySection.h"
#include "VectorLibrary.h"

MeshGeneratorBySection::MeshGeneratorBySection()
{
}

MeshGeneratorBySection::~MeshGeneratorBySection()
{
}

MeshGeneratorBySection::MeshGeneratorBySection(CrossSection& section, float meshHeight)
{
	this->sectionPtr = &section;
	this->meshHeight = meshHeight;
}

void MeshGeneratorBySection::addSection(CrossSection& section, float z)
{
	//��ӽڵ�λ�õ���ʼid
	uint32 beginId = this->Vertices.Num();
	//���ٽڵ�ĸ���
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//����ÿ��Ҫ��ӵĽڵ�
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
	{
		auto& currPoint = section.vertexList[idVertex];
		SetVertex(beginId + idVertex, { currPoint.X, currPoint.Y, z });
	}
}

void MeshGeneratorBySection::addSection(CrossSection& section, FTransform3f& transform)
{
	//��ӽڵ����ʼλ��id
	uint32 beginId = this->Vertices.Num();
	//���ٶ�Ӧ�Ľڵ����
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//����ÿ��Ҫ��ӵĽڵ�
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex) {
		//ת����ǰλ�õĵ�
		FVector3f transformedPoint = transform.TransformPosition(
			VectorLibrary::convertVector<FVector, FVector3f>(section.vertexList[idVertex]));
		//���ת����Ľڵ�
		SetVertex(beginId + idVertex,
			VectorLibrary::convertVector<FVector3f, FVector>(transformedPoint));
	}
}

void MeshGeneratorBySection::addSection(CrossSection& section, FVector3f center, FVector2i idMap)
{
	//��ӽڵ����ʼλ��id
	uint32 beginId = this->Vertices.Num();
	//���ٶ�Ӧ�Ľڵ����
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//����ÿ��Ҫ��ӵĽڵ�
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex) {
		FVector tempPoint = { 0,0,0 };
		auto& srcPoint = section.vertexList[idVertex];
		tempPoint[idMap[0]] = srcPoint.X;
		tempPoint[idMap[1]] = srcPoint.Y;
		//���ڵ����ƫ����
		tempPoint.X += center.X;
		tempPoint.Y += center.Y;
		tempPoint.Z += center.Z;
		//���ת����Ľڵ�
		SetVertex(beginId + idVertex, tempPoint);
	}
}

void MeshGeneratorBySection::makeMeshBySection()
{
	//�����section
	addSection(*sectionPtr, 0);
	addSection(*sectionPtr, meshHeight);
	//�պ������section
	closeSection(0, *sectionPtr, false);
	closeSection(1, *sectionPtr, true);
	//������������
	linkSection(*sectionPtr, 1);
}

void MeshGeneratorBySection::makeArchMesh()
{
	//�������εİ뾶
	float archRadius = 10.f;
	//ֱ�߲��ֵĳ���
	float straightLength = 100.f;
	//��������Ĳ�������
	uint32 archSampleNum = 8;
	//���㻡��ʱÿһ���ĽǶ�
	float archStep = PI / 2 / archSampleNum;
	//���β���Բ�����ڵ�λ��
	//���軡������XOZƽ���ϵģ������ʼ������Z������
	FVector3f archCenter(archRadius, 0, straightLength + archRadius);
	//��Ӵ�ֱ��ֱ������
	addSection(*this->sectionPtr, 0);
	addSection(*this->sectionPtr, straightLength);
	//����ÿһ��ʱ��mesh�����ĵ�����
	FVector3f meshCenter;
	//��ӻ�������
	for (uint32 idArch = 1; idArch <= archSampleNum; ++idArch) {
		//��ǰ��ĽǶ�
		float currentAngle = archStep * idArch;
		//��ǰ��mesh���ĵ�����
		float x = archRadius - archRadius * std::cos(currentAngle);
		float z = straightLength + archRadius * std::sin(currentAngle);
		//����ȷ�ϵ�ǰmesh�����ĵ�
		meshCenter = { x,0,z };
		//���㵱ǰmesh����ת
		//pitch Y, Yaw Z, Roll, X
		//����ط�Ҫ����ת���ɽǶ���ʽ
		FRotator3f rotation((-currentAngle)/PI*180, 0, 0);
		//FRotator3f rotation(0, 0, 0);
		//�½���ת�任
		FTransform3f transform(rotation, meshCenter, FVector3f(1, 1, 1));
		//����transform���section
		this->addSection(*this->sectionPtr, transform);
	}
	//���ˮƽ�ε���ת
	FRotator3f horizontalRotation(-90, 0, 0);
	FTransform3f horizontalTrans(horizontalRotation,
		FVector3f(archRadius + straightLength, 0, archRadius + straightLength));
	this->addSection(*this->sectionPtr, horizontalTrans);
	this->addSection(*this->sectionPtr, { archRadius + straightLength, 0, archRadius + straightLength },
		{ 1,2 });
	//Ŀǰ�Ѿ���ӹ���section����
	uint32 sectionNum = this->Vertices.Num() / this->sectionPtr->vertexList.size();
	//����Ϊֹ����������˽ڵ㣬����û�����ӹ��ڵ�
	for (uint32 idSection = 1; idSection < sectionNum; ++idSection) {
		linkSection(*this->sectionPtr, idSection);
	}
}

void MeshGeneratorBySection::closeSection(uint32 idSection, CrossSection& section, bool clockFlag)
{
	//��������ε���ʼid
	uint32 idTriangle = this->Triangles.Num();
	//Ҫ��ӵ���Ƭ�ĸ���
	uint32 triangleNum = section.vertexList.size() - 2;
	//Ŀǰ������Ҫ��ӵķ�������UV��Ϣ��triangle�ĸ���Ӧ����һ�µ�
	this->ExtendBufferSizes(0, triangleNum, triangleNum, triangleNum);
	//��������ÿ��������Ƭ
	int beginId = idSection * section.vertexList.size();
	//��¼Ŀǰ��face id
	uint32 currFaceId = getNewFaceId();
	//��ÿ���ڵ����UV id
	this->uvManager.beginRegisterUV();
	//����section�����ÿ���㣬������ע����Щ���UV id
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
	{
		this->uvManager.registerUV(idVertex + beginId);
	}
	this->uvManager.endRegisterUV(*this);
	//׼��UV�ڵ�ķ�����������Ҫ�Ǹ���˳ʱ�뻹����ʱ������
	FVector3f vertexNormal = clockFlag ? FVector3f(0, 0, 1) : FVector3f(0, 0, -1);
	//����ÿ���ڵ�ķ�������UV
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
	{
		//��ǰλ�õ�ʵ��vertex id
		uint32 currVertexId = idVertex + beginId;
		//��ȡ��Ӧ��uv id
		uint32 currUVId = this->uvManager.getUV(currVertexId);
		//����UV�ڵ�ķ�����
		Normals[currUVId] = vertexNormal;
		//����uv�ڵ����Ⱦλ��
		UVs[currUVId] = section.normalVertex[idVertex];
	}
	//��0��ʼ�������е�������
	for (int id = 1; id + 1 < section.vertexList.size(); ++id)
	{
		int idVertex = beginId + id;
		if (clockFlag)
		{
			this->SetTriangle(idTriangle, { idVertex + 1, idVertex, beginId });
			//������������ε�UV
			this->SetTriangleUVs(idTriangle,
				this->uvManager.getUV(idVertex + 1),
				this->uvManager.getUV(idVertex),
				this->uvManager.getUV(beginId)
			);
			//���������ε�normal
			this->SetTriangleNormals(idTriangle,
				this->uvManager.getUV(idVertex + 1),
				this->uvManager.getUV(idVertex),
				this->uvManager.getUV(beginId)
			);
		}
		else
		{
			this->SetTriangle(idTriangle, { beginId, idVertex, idVertex + 1 });
			//���������ε�UV
			this->SetTriangleUVs(idTriangle,
				this->uvManager.getUV(beginId),
				this->uvManager.getUV(idVertex),
				this->uvManager.getUV(idVertex + 1)
			);
			this->SetTriangleNormals(idTriangle,
				this->uvManager.getUV(beginId),
				this->uvManager.getUV(idVertex),
				this->uvManager.getUV(idVertex + 1)
			);
		}
		//����������������face
		this->SetTrianglePolygon(idTriangle, currFaceId);
		++idTriangle;
	}
}

uint32 MeshGeneratorBySection::getNewFaceId()
{
	++faceNum;
	return faceNum - 1;
}

void MeshGeneratorBySection::linkSection(CrossSection& section, int idSection)
{
	//��ǰ��mesh�����ܵĽڵ����
	int vertexNum = Vertices.Num();
	//��õ�ǰsection����ʼid ���ʾ��ǰsection�������ڵ����ʼid
	int beginVertexId = section.vertexList.size() * idSection;
	//��һ��section �Ľڵ�����id
	int lastBeginId = section.vertexList.size() * (idSection - 1);
	//���������ʱ�������ε���ʼid
	int idTriangle = this->Triangles.Num();
	//��չ��Ҫ���ӵ�triangle����
	uint32 needExtendNum = section.vertexList.size() * 2;
	this->ExtendBufferSizes(0, needExtendNum, needExtendNum, needExtendNum);
	//����ÿ��id ��ָ���ǰ�����λ�õ�section��������
	for (int id = 0; id < section.vertexList.size(); ++id)
	{
		uint32 currFace = getNewFaceId();
		//��һ��λ�õ�id
		int nextId = (id + 1) % section.vertexList.size();
		//�ȸ���ǰface��ÿ����ע��UV id
		this->uvManager.beginRegisterUV();
		//�������ڵ�ǰ�����εĽڵ㣬���UV
		this->uvManager.registerUV(beginVertexId + id);
		this->uvManager.registerUV(beginVertexId + nextId);
		this->uvManager.registerUV(lastBeginId + id);
		this->uvManager.registerUV(lastBeginId + nextId);
		this->uvManager.endRegisterUV(*this);
		//���㵱ǰ����ķ�����
		FVector3f tempNormal = VectorLibrary::getPointNormal<FVector3f>(
			this->Vertices[beginVertexId + id],
			this->Vertices[lastBeginId + id],
			this->Vertices[beginVertexId + nextId]
		);
		//��ÿ��UV����normal
		this->uvManager.setNormalToUV(tempNormal, *this);
		//����UV��Ӧ����Ⱦ����
		//����ֱ�Ӹ��ݾ��ε��߼���������Ⱦ˳��
		this->uvManager.setRectangleUV(
			beginVertexId + id,
			beginVertexId + nextId,
			lastBeginId + id,
			lastBeginId + nextId,
			*this
		);
		//��������ζ�Ӧ��uv
		this->addTriangleOnce(idTriangle++, beginVertexId + id,
			beginVertexId + nextId, lastBeginId + id, currFace);
		//��Ӳ�ߺ�������һ��������
		this->addTriangleOnce(idTriangle++, beginVertexId + nextId,
			lastBeginId + nextId, lastBeginId + id, currFace);
	}
}

void MeshGeneratorBySection::addTriangleOnce(uint32 idTriangle, uint32 id0, uint32 id1, uint32 id2, uint32 faceId)
{
	//����triangle
	this->SetTriangle(idTriangle, id0, id1, id2);
	//����id��ӳ��λ�����������uv
	this->SetTriangleUVs(idTriangle, this->uvManager.getUV(id0),
		this->uvManager.getUV(id1),
		this->uvManager.getUV(id2)
	);
	//���������εķ���������UV���߼���һ����
	this->SetTriangleNormals(idTriangle, this->uvManager.getUV(id0),
		this->uvManager.getUV(id1),
		this->uvManager.getUV(id2)
	);
	//������������ڵĶ����id
	this->SetTrianglePolygon(idTriangle, faceId);
}

FMeshShapeGenerator& MeshGeneratorBySection::Generate()
{
	//����section����mesh
	if (generateMethod == CYLINDER)
		this->makeMeshBySection();
	else
		this->makeArchMesh();
	return *this;
}
