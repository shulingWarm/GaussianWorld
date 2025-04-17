// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator2.h"
#include "CrossSection.h"
#include "TextureEffect.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "DynamicMesh/MeshTransforms.h"
#include "TextureConfigLibrary.h"
#include<unordered_map>
#include "DynamicMeshEditor.h"
#include "Generators/GridBoxMeshGenerator.h"

using MeshDataType = UE::Geometry::FDynamicMesh3;
using namespace UE::Geometry;

//��ʱ�����mesh������
class MeshManager
{
public:
	//mesh������
	MeshDataType* meshData;
	//Ŀǰ��ӹ���ÿ��vertex�Ľڵ�
	std::vector<int> vertexIdList;

	//��������ӽڵ�
	void addVertex(FVector vertex)
	{
		int tempId = meshData->AppendVertex(vertex);
		vertexIdList.push_back(tempId);
	}

	//��������εĽӿ�
	void addTriangle(int id1, int id2, int id3)
	{
		meshData->AppendTriangle(vertexIdList[id1],
			vertexIdList[id2], vertexIdList[id3]);
	}
};

void makeRectangleSection(CrossSection& crossSection)
{
	//����������д���ĸ���
	crossSection.addVertex({ 0,0,0 });
	crossSection.addVertex({ 200,0,0 });
	crossSection.addVertex({ 200,200,0 });
	crossSection.addVertex({ 0,200,0 });
	//����section�Ĺ�һ������
	crossSection.generateNormalCoord();
}
//
//��mesh��������Ӻ����
static void addSectionToLine(CrossSection& section,
	float sectionZ, //Ŀǰֻ����Z����ǣ�������˵Ӧ���ñ任��������ʾ
	MeshManager& meshManager
)
{
	//����section�����ÿ����
	for (auto& eachPoint : section.vertexList)
	{
		meshManager.addVertex({ eachPoint[0], eachPoint[1], sectionZ });
	}
}

//��������section
static void linkSection(CrossSection& section,
	MeshManager& meshManager,
	int idSection //�������section��������һ��section
)
{
	//��ǰ��mesh�����ܵĽڵ����
	int vertexNum = meshManager.vertexIdList.size();
	//��õ�ǰsection����ʼid
	int beginVertexId = section.vertexList.size() * idSection;
	//��һ��section����id
	int lastBeginId = section.vertexList.size() * (idSection - 1);
	//����ÿ��id
	for (int id = 0; id < section.vertexList.size(); ++id)
	{
		//��һ��λ�õ�id
		int nextId = (id + 1) % section.vertexList.size();
		//��������id
		meshManager.addTriangle(beginVertexId + id, beginVertexId + nextId, lastBeginId + id);
		meshManager.addTriangle(beginVertexId + nextId, lastBeginId + nextId, lastBeginId + id);
	}
}

//���ĳһ������
static void closeSection(CrossSection& crossSection, MeshManager& meshManager, int idSection, bool clockFlag)
{
	int beginId = idSection * crossSection.vertexList.size();
	//��0��ʼ�������е�������
	for (int id = 1; id + 1 < crossSection.vertexList.size(); ++id)
	{
		int idVertex = beginId + id;
		if (clockFlag)
		{
			meshManager.addTriangle(idVertex + 1, idVertex, beginId);
		}
		else
		{
			meshManager.addTriangle(beginId, idVertex, idVertex + 1);
		}
	}
}

//��mesh���һ��box�Ĳ��Ժ���
static void addBoxToMesh(FDynamicMesh3& meshData)
{
	//�½�mesh������
	MeshManager meshManager;
	meshManager.meshData = &(meshData);
	//���������εĺ����
	CrossSection crossSection;
	makeRectangleSection(crossSection);
	//��mesh������Ӻ����
	addSectionToLine(crossSection, 0, meshManager);
	//��պ����
	closeSection(crossSection, meshManager, 0, false);
	//��mesh���������һ�������
	addSectionToLine(crossSection, 100, meshManager);
	//���section
	closeSection(crossSection, meshManager, 1, true);
	//��������section
	linkSection(crossSection, meshManager, 1);
}

static void LocalAppendPrimitive(
	UDynamicMesh* TargetMesh,
	FMeshShapeGenerator* Generator,
	FTransform Transform,
	FVector3d PreTranslate)
{
	auto ApplyOptionsToMesh = [&Transform, PreTranslate](FDynamicMesh3& Mesh)
		{
			if (PreTranslate.SquaredLength() > 0)
			{
				MeshTransforms::Translate(Mesh, PreTranslate);
			}

			MeshTransforms::ApplyTransform(Mesh, (FTransformSRT3d)Transform, true);
		};

	if (TargetMesh->IsEmpty())
	{
		TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
			{
				EditMesh.Copy(Generator);
				ApplyOptionsToMesh(EditMesh);
			}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	}
	else
	{
		FDynamicMesh3 TempMesh(Generator);
		ApplyOptionsToMesh(TempMesh);
		TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
			{
				FMeshIndexMappings TmpMappings;
				FDynamicMeshEditor Editor(&EditMesh);
				Editor.AppendMesh(&TempMesh, TmpMappings);

			}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	}
}

//Ŀǰ�Ѿ��������UV���ݹ�����
class VertexUVManager
{
public:
	//Ŀǰ�Ѿ���������ܵ�UV�ĸ���
	uint32 totalUvId = 0;
	//��ǰ״̬�µ�ÿ���ڵ�id��Ӧ��UVӳ���
	std::unordered_map<uint32, uint32> vertexUVMap;

	//��ǿ�ʼע��UV
	void beginRegisterUV()
	{
		//���Ŀǰ�Ľڵ��UVӳ���
		this->vertexUVMap.clear();
	}

	//ע��ڵ�
	void registerUV(uint32 idVertex)
	{
		//�ж�map�����Ƿ��Ѿ���������ڵ�
		if (this->vertexUVMap.count(idVertex) == 0)
		{
			this->vertexUVMap[idVertex] = this->totalUvId;
			++(this->totalUvId);
		}
	}

	//������UV�ڵ��ע��
	void endRegisterUV(FMeshShapeGenerator& generator)
	{
		//��generator��չλ��
		generator.ExtendBufferSizes(0, 0, this->vertexUVMap.size(),
			this->vertexUVMap.size());
		//������˵��չ��֮��Ӧ�ú�total�ĸ�����һ����
		if (this->totalUvId != generator.UVs.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("this->totalUvId != generator.UVs.Num()"));
		}
		//��չ��λ��֮�󣬸�ÿ��UV��normal�����������Ľڵ�
		for (auto& eachPair : this->vertexUVMap)
		{
			generator.UVParentVertex[eachPair.second] = eachPair.first;
			generator.NormalParentVertex[eachPair.second] = eachPair.first;
		}
	}

	//����ǰע���ÿ��UV����normal
	void setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator)
	{
		//������ǰ��¼������ÿ��UV
		for (auto& eachPair : this->vertexUVMap)
		{
			generator.Normals[eachPair.second] = normal;
		}
	}

	//���þ��ε���Ⱦ����
	//�����������Ҫ����UVһ����4��
	//һ�����������������Ⱦ�����ʱ����������Ĳ���
	void setRectangleUV(
		//������λ�ھ����ĸ��ǵ�UV����
		//���������洫�����ԭʼ��vertex id
		//������Ҫ�Լ������ĳ�uv id��
		uint32 id00,
		uint32 id10,//����x=1, y=0
		uint32 id01,
		uint32 id11,
		FMeshShapeGenerator& generator
	)
	{
		//��������ÿ����Ӧλ�õ�uv id
		generator.UVs[this->getUV(id00)] = { 0,0 };
		generator.UVs[this->getUV(id10)] = { 1,0 };
		generator.UVs[this->getUV(id01)] = { 0,1 };
		generator.UVs[this->getUV(id11)] = { 1,1 };
	}

	//��ýڵ��Ӧ��UV
	uint32 getUV(uint32 idVertex)
	{
		if (this->vertexUVMap.count(idVertex) == 0)
			return 0;
		return this->vertexUVMap.at(idVertex);
	}
};

//������������㷨����
template<class RetType>
RetType getPointNormal(
	FVector point1,
	FVector point2,
	FVector point3
)
{
	//12������ķ�����
	FVector vec12 = point2 - point1;
	//��2��3������
	FVector vec23 = point3 - point2;
	//�������������Ĳ��
	FVector ret = FVector::CrossProduct(vec12, vec23);
	return RetType(ret.X, ret.Y, ret.Z);
}

//�Զ���� shape generator
class MyBoxGenerator : public FMeshShapeGenerator
{
public:
	/** 3D box */
	FOrientedBox3d Box;

	FIndex3i EdgeVertices{ 8,8,8 };

	/** If true (default), UVs are scaled so that there is no stretching. If false, UVs are scaled to fill unit square */
	bool bScaleUVByAspectRatio = true;

	/** If true, each quad of box gets a separate polygroup */
	bool bPolygroupPerQuad = false;

	//��ǰ�Ľڵ�����������UV������
	VertexUVManager uvManager;
	//Ŀǰ��face����
	uint32 faceNum = 0;

public:

	//��ָ��Z��λ����ӽ���
	void addSection(float z, CrossSection& section)
	{
		//��ӽڵ�λ�õ���ʼid
		uint32 beginId = this->Vertices.Num();
		//���ٽڵ�ĸ���
		ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
		//����ÿ��Ҫ��ӵĽڵ�
		for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
		{
			auto& currPoint = section.vertexList[idVertex];
			SetVertex(beginId + idVertex, Box.Frame.PointAt({ currPoint.X, currPoint.Y, z }));
		}
	}

	//����һ�������������
	void addTriangleOnce(
		uint32 idTriangle ,uint32 id0, uint32 id1, uint32 id2, uint32 faceId
	)
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

	//ע�Ტ����µ�face id
	uint32 getNewFaceId()
	{
		++faceNum;
		return faceNum - 1;
	}

	//����section���߼�
	void linkSection(CrossSection& section,
		int idSection //�������section��������һ��section
	)
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
			FVector3f tempNormal = getPointNormal<FVector3f>(
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

	//���ָ��λ�õĽ���
	//����Ҫָ��Ҫ����յĽ����id
	void closeSection(uint32 idSection, CrossSection& section, bool clockFlag)
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

	//����������Ӧ��8����
	void addBoxVertex()
	{
		//׼������������
		CrossSection crossSection;
		makeRectangleSection(crossSection);
		//��ָ����Z��λ������������
		addSection(0, crossSection);
		addSection(100, crossSection);
		//���ָ���Ľ���
		closeSection(0, crossSection, false);
		//����½���
		closeSection(1, crossSection, true);
		//������������
		linkSection(crossSection, 1);
	}

	inline void LocalSetTriangle(int Index, int A, int B, int C)
	{
		UE_LOG(LogTemp, Warning, TEXT("Triangle: %d %d %d %d\n"), Index, A, B, C);
		SetTriangle(Index, A, B, C);
	}

	inline void LocalSetTriangleUVs(int Index, int A, int B, int C)
	{
		UE_LOG(LogTemp, Warning, TEXT("UV: %d %d %d %d\n"), Index, A, B, C);
		SetTriangleUVs(Index, A, B, C);
	}

	inline void LocalSetTriangleNormals(int Index, int A, int B, int C)
	{
		UE_LOG(LogTemp, Warning, TEXT("Normals: %d %d %d %d\n"), Index, A, B, C);
		SetTriangleNormals(Index, A, B, C);
	}

	inline void LocalSetTrianglePolygon(int Index, int PolygonID)
	{
		UE_LOG(LogTemp, Warning, TEXT("Polygon: %d %d\n"), Index, PolygonID);
		SetTrianglePolygon(Index, PolygonID);
	}

	//����һ��������ƽ�棬����Ϊ�˼���Լ����ڵ���������Ƭ������߼��Ƿ������ȷ��
	void addTestTriangle()
	{
		//���������
		this->ExtendBufferSizes(3, 1, 1, 1);
		//������������
		this->SetVertex(0, Box.Frame.PointAt({ 0,0,0 }));
		this->SetVertex(1, Box.Frame.PointAt({ 0,500,0 }));
		this->SetVertex(2, Box.Frame.PointAt({ 500,0,0 }));
		//��������ע��UV
		this->uvManager.beginRegisterUV();
		this->uvManager.registerUV(0);
		this->uvManager.registerUV(1);
		this->uvManager.registerUV(2);
		//������vertex��UVע��
		this->uvManager.endRegisterUV(*this);
		//����������
		this->SetTriangle(0, 0, 1, 2);
		//����UV
		this->SetTriangleUVs(0, uvManager.getUV(0),
			uvManager.getUV(1),
			uvManager.getUV(2)
		);
		this->SetTriangleNormals(0, uvManager.getUV(0),
			uvManager.getUV(1),
			uvManager.getUV(2)
		);
		//�������������ڵĶ����
		this->SetTrianglePolygon(0, 0);
		//����UV����Ⱦƽ���λ��
		this->UVs[uvManager.getUV(0)] = { 0,0 };
		this->UVs[uvManager.getUV(1)] = { 1,0 };
		this->UVs[uvManager.getUV(2)] = { 0,1 };
		//����ÿ��UV�ķ�����
		this->Normals[uvManager.getUV(0)] = { 0,0,1 };
		this->Normals[uvManager.getUV(1)] = { 0,0,1 };
		this->Normals[uvManager.getUV(2)] = { 0,0,1 };
	}

	/** Generate the mesh */
	virtual FMeshShapeGenerator& Generate() override
	{
		addBoxVertex();
		return *this;
	}
};

void AMeshGenerator2::makeDynamicMesh(UDynamicMesh* TargetMesh)
{
	if (TargetMesh == nullptr)
	{
		return;
	}

	float DimensionX = 100;
	float DimensionY = 100;
	float DimensionZ = 100;
	int32 StepsX = 0;
	int32 StepsY = 0;
	int32 StepsZ = 0;

	UE::Geometry::FAxisAlignedBox3d ConvertBox(
		FVector3d(-DimensionX / 2, -DimensionY / 2, 0),
		FVector3d(DimensionX / 2, DimensionY / 2, DimensionZ));

	// todo: if Steps X/Y/Z are zero, can use trivial box generator

	MyBoxGenerator GridBoxGenerator;
	GridBoxGenerator.Box = UE::Geometry::FOrientedBox3d(ConvertBox);
	// ��ʼ���� box ֮���������ӡ box����� extends
	UE_LOG(LogTemp, Warning, TEXT("Box extend: %f %f %f\n"),
		GridBoxGenerator.Box.Extents.X, GridBoxGenerator.Box.Extents.Y, GridBoxGenerator.Box.Extents.Z);
	GridBoxGenerator.EdgeVertices = FIndex3i(FMath::Max(0, StepsX), FMath::Max(0, StepsY), FMath::Max(0, StepsZ));
	GridBoxGenerator.bPolygroupPerQuad = false;
	GridBoxGenerator.Generate();

	FVector3d OriginShift = FVector3d::Zero();
	LocalAppendPrimitive(TargetMesh, &GridBoxGenerator, FTransform(), OriginShift);
}

UTexture2D* AMeshGenerator2::makeTestTexture()
{
	//����Ŀ��
	constexpr uint32 texWidth = 800;
	constexpr uint32 texHeight = 800;
	//����һ���յ�texture
	UTexture2D* retTexture = UTexture2D::CreateTransient(
		texWidth, texHeight, PF_B8G8R8A8
	);
	//��texture���ݵĻ�������
	TextureConfigLibrary::baseConfigTexture(*retTexture);
	//��ȡtexture����
	auto textureData = TextureConfigLibrary::getTextureData<uint8>(*retTexture);
	//��������������Ʋ�Ч��
	TextureEffect::PSCloudEffect(textureData, texWidth, texHeight,
		TextureEffect::Vec3(19, 11, 0), TextureEffect::Vec3(50, 41, 6));
	//���Ʋ������ɫ
	TextureEffect::addNoise(textureData, texWidth, texHeight,
		{ 200, 200, 200 }, 0.4
	);
	//��ͼƬ������ģ��
	TextureEffect::motionBlur(textureData, texWidth, texHeight);
	//����texture����
	TextureConfigLibrary::unlockTexture(*retTexture);
	//��texture���ݵ���β����
	TextureConfigLibrary::textureFinalUpdate(*retTexture);
	return retTexture;
}
