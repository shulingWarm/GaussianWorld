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

//临时处理的mesh管理器
class MeshManager
{
public:
	//mesh的数据
	MeshDataType* meshData;
	//目前添加过的每个vertex的节点
	std::vector<int> vertexIdList;

	//向里面添加节点
	void addVertex(FVector vertex)
	{
		int tempId = meshData->AppendVertex(vertex);
		vertexIdList.push_back(tempId);
	}

	//添加三角形的接口
	void addTriangle(int id1, int id2, int id3)
	{
		meshData->AppendTriangle(vertexIdList[id1],
			vertexIdList[id2], vertexIdList[id3]);
	}
};

void makeRectangleSection(CrossSection& crossSection)
{
	//在里面依次写入四个点
	crossSection.addVertex({ 0,0,0 });
	crossSection.addVertex({ 200,0,0 });
	crossSection.addVertex({ 200,200,0 });
	crossSection.addVertex({ 0,200,0 });
	//生成section的归一化坐标
	crossSection.generateNormalCoord();
}
//
//向mesh线里面添加横截面
static void addSectionToLine(CrossSection& section,
	float sectionZ, //目前只是用Z来标记，正常来说应该用变换矩阵来表示
	MeshManager& meshManager
)
{
	//遍历section里面的每个点
	for (auto& eachPoint : section.vertexList)
	{
		meshManager.addVertex({ eachPoint[0], eachPoint[1], sectionZ });
	}
}

//链接两组section
static void linkSection(CrossSection& section,
	MeshManager& meshManager,
	int idSection //连接这个section和它的上一个section
)
{
	//当前的mesh里面总的节点个数
	int vertexNum = meshManager.vertexIdList.size();
	//获得当前section的起始id
	int beginVertexId = section.vertexList.size() * idSection;
	//上一个section步进id
	int lastBeginId = section.vertexList.size() * (idSection - 1);
	//步进每个id
	for (int id = 0; id < section.vertexList.size(); ++id)
	{
		//下一个位置的id
		int nextId = (id + 1) % section.vertexList.size();
		//连接两个id
		meshManager.addTriangle(beginVertexId + id, beginVertexId + nextId, lastBeginId + id);
		meshManager.addTriangle(beginVertexId + nextId, lastBeginId + nextId, lastBeginId + id);
	}
}

//封闭某一个截面
static void closeSection(CrossSection& crossSection, MeshManager& meshManager, int idSection, bool clockFlag)
{
	int beginId = idSection * crossSection.vertexList.size();
	//从0开始连接所有的三角形
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

//给mesh添加一个box的测试函数
static void addBoxToMesh(FDynamicMesh3& meshData)
{
	//新建mesh管理器
	MeshManager meshManager;
	meshManager.meshData = &(meshData);
	//生成正方形的横截面
	CrossSection crossSection;
	makeRectangleSection(crossSection);
	//向mesh里面添加横截面
	addSectionToLine(crossSection, 0, meshManager);
	//封闭横截面
	closeSection(crossSection, meshManager, 0, false);
	//向mesh里面添加另一个横截面
	addSectionToLine(crossSection, 100, meshManager);
	//封闭section
	closeSection(crossSection, meshManager, 1, true);
	//链接两组section
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

//目前已经分配过的UV数据管理器
class VertexUVManager
{
public:
	//目前已经分配过的总的UV的个数
	uint32 totalUvId = 0;
	//当前状态下的每个节点id对应的UV映射表
	std::unordered_map<uint32, uint32> vertexUVMap;

	//标记开始注册UV
	void beginRegisterUV()
	{
		//清除目前的节点的UV映射表
		this->vertexUVMap.clear();
	}

	//注册节点
	void registerUV(uint32 idVertex)
	{
		//判断map里面是否已经有了这个节点
		if (this->vertexUVMap.count(idVertex) == 0)
		{
			this->vertexUVMap[idVertex] = this->totalUvId;
			++(this->totalUvId);
		}
	}

	//结束对UV节点的注册
	void endRegisterUV(FMeshShapeGenerator& generator)
	{
		//给generator扩展位置
		generator.ExtendBufferSizes(0, 0, this->vertexUVMap.size(),
			this->vertexUVMap.size());
		//正常来说扩展完之后应该和total的个数是一样的
		if (this->totalUvId != generator.UVs.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("this->totalUvId != generator.UVs.Num()"));
		}
		//扩展好位置之后，给每个UV和normal绑定它们所属的节点
		for (auto& eachPair : this->vertexUVMap)
		{
			generator.UVParentVertex[eachPair.second] = eachPair.first;
			generator.NormalParentVertex[eachPair.second] = eachPair.first;
		}
	}

	//给当前注册的每个UV设置normal
	void setNormalToUV(FVector3f normal, FMeshShapeGenerator& generator)
	{
		//遍历当前记录下来的每个UV
		for (auto& eachPair : this->vertexUVMap)
		{
			generator.Normals[eachPair.second] = normal;
		}
	}

	//设置矩形的渲染坐标
	//这种情况下需要假设UV一共就4个
	//一般在做横截面连接渲染侧面的时候会有这样的操作
	void setRectangleUV(
		//下面是位于矩形四个角的UV坐标
		//而且这里面传入的是原始的vertex id
		//这是需要自己把它改成uv id的
		uint32 id00,
		uint32 id10,//这是x=1, y=0
		uint32 id01,
		uint32 id11,
		FMeshShapeGenerator& generator
	)
	{
		//依次设置每个对应位置的uv id
		generator.UVs[this->getUV(id00)] = { 0,0 };
		generator.UVs[this->getUV(id10)] = { 1,0 };
		generator.UVs[this->getUV(id01)] = { 0,1 };
		generator.UVs[this->getUV(id11)] = { 1,1 };
	}

	//获得节点对应的UV
	uint32 getUV(uint32 idVertex)
	{
		if (this->vertexUVMap.count(idVertex) == 0)
			return 0;
		return this->vertexUVMap.at(idVertex);
	}
};

//根据三个点计算法向量
template<class RetType>
RetType getPointNormal(
	FVector point1,
	FVector point2,
	FVector point3
)
{
	//12两个点的法向量
	FVector vec12 = point2 - point1;
	//从2到3的向量
	FVector vec23 = point3 - point2;
	//计算两个向量的叉乘
	FVector ret = FVector::CrossProduct(vec12, vec23);
	return RetType(ret.X, ret.Y, ret.Z);
}

//自定义的 shape generator
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

	//当前的节点管理器里面的UV管理器
	VertexUVManager uvManager;
	//目前的face个数
	uint32 faceNum = 0;

public:

	//在指定Z轴位置添加截面
	void addSection(float z, CrossSection& section)
	{
		//添加节点位置的起始id
		uint32 beginId = this->Vertices.Num();
		//开辟节点的个数
		ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
		//遍历每个要添加的节点
		for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
		{
			auto& currPoint = section.vertexList[idVertex];
			SetVertex(beginId + idVertex, Box.Frame.PointAt({ currPoint.X, currPoint.Y, z }));
		}
	}

	//集成一次性添加三角形
	void addTriangleOnce(
		uint32 idTriangle ,uint32 id0, uint32 id1, uint32 id2, uint32 faceId
	)
	{
		//设置triangle
		this->SetTriangle(idTriangle, id0, id1, id2);
		//根据id的映射位置往里面添加uv
		this->SetTriangleUVs(idTriangle, this->uvManager.getUV(id0),
			this->uvManager.getUV(id1),
			this->uvManager.getUV(id2)
		);
		//设置三角形的法向量，和UV的逻辑是一样的
		this->SetTriangleNormals(idTriangle, this->uvManager.getUV(id0),
			this->uvManager.getUV(id1),
			this->uvManager.getUV(id2)
		);
		//添加三角形所在的多边形id
		this->SetTrianglePolygon(idTriangle, faceId);
	}

	//注册并获得新的face id
	uint32 getNewFaceId()
	{
		++faceNum;
		return faceNum - 1;
	}

	//连接section的逻辑
	void linkSection(CrossSection& section,
		int idSection //连接这个section和它的上一个section
	)
	{
		//当前的mesh里面总的节点个数
		int vertexNum = Vertices.Num();
		//获得当前section的起始id 这表示当前section的所属节点的起始id
		int beginVertexId = section.vertexList.size() * idSection;
		//上一个section 的节点所属id
		int lastBeginId = section.vertexList.size() * (idSection - 1);
		//添加三角形时，三角形的起始id
		int idTriangle = this->Triangles.Num();
		//扩展需要增加的triangle数量
		uint32 needExtendNum = section.vertexList.size() * 2;
		this->ExtendBufferSizes(0, needExtendNum, needExtendNum, needExtendNum);
		//步进每个id 这指的是把相连位置的section连接起来
		for (int id = 0; id < section.vertexList.size(); ++id)
		{
			uint32 currFace = getNewFaceId();
			//下一个位置的id
			int nextId = (id + 1) % section.vertexList.size();
			//先给当前face的每个点注册UV id
			this->uvManager.beginRegisterUV();
			//遍历属于当前三角形的节点，添加UV
			this->uvManager.registerUV(beginVertexId + id);
			this->uvManager.registerUV(beginVertexId + nextId);
			this->uvManager.registerUV(lastBeginId + id);
			this->uvManager.registerUV(lastBeginId + nextId);
			this->uvManager.endRegisterUV(*this);
			//计算当前侧面的法向量
			FVector3f tempNormal = getPointNormal<FVector3f>(
				this->Vertices[beginVertexId + id],
				this->Vertices[lastBeginId + id],
				this->Vertices[beginVertexId + nextId]
			);
			//给每个UV设置normal
			this->uvManager.setNormalToUV(tempNormal, *this);
			//设置UV对应的渲染坐标
			//这里直接根据矩形的逻辑来设置渲染顺序
			this->uvManager.setRectangleUV(
				beginVertexId + id,
				beginVertexId + nextId,
				lastBeginId + id,
				lastBeginId + nextId,
				*this
			);
			//添加三角形对应的uv
			this->addTriangleOnce(idTriangle++, beginVertexId + id, 
				beginVertexId + nextId, lastBeginId + id, currFace);
			//添加侧边横截面的另一个三角形
			this->addTriangleOnce(idTriangle++, beginVertexId + nextId,
				lastBeginId + nextId, lastBeginId + id, currFace);
		}
	}

	//封闭指定位置的截面
	//这需要指定要被封闭的截面的id
	void closeSection(uint32 idSection, CrossSection& section, bool clockFlag)
	{
		//添加三角形的起始id
		uint32 idTriangle = this->Triangles.Num();
		//要添加的面片的个数
		uint32 triangleNum = section.vertexList.size() - 2;
		//目前来看，要添加的法向量和UV信息和triangle的个数应该是一致的
		this->ExtendBufferSizes(0, triangleNum, triangleNum, triangleNum);
		//依次设置每个三角面片
		int beginId = idSection * section.vertexList.size();
		//记录目前的face id
		uint32 currFaceId = getNewFaceId();
		//给每个节点分配UV id
		this->uvManager.beginRegisterUV();
		//遍历section里面的每个点，在里面注册这些点的UV id
		for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
		{
			this->uvManager.registerUV(idVertex + beginId);
		}
		this->uvManager.endRegisterUV(*this);
		//准备UV节点的法向量，这主要是根据顺时针还是逆时针来的
		FVector3f vertexNormal = clockFlag ? FVector3f(0, 0, 1) : FVector3f(0, 0, -1);
		//设置每个节点的法向量和UV
		for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
		{
			//当前位置的实际vertex id
			uint32 currVertexId = idVertex + beginId;
			//获取对应的uv id
			uint32 currUVId = this->uvManager.getUV(currVertexId);
			//设置UV节点的法向量
			Normals[currUVId] = vertexNormal;
			//设置uv节点的渲染位置
			UVs[currUVId] = section.normalVertex[idVertex];
		}
		//从0开始连接所有的三角形
		for (int id = 1; id + 1 < section.vertexList.size(); ++id)
		{
			int idVertex = beginId + id;
			if (clockFlag)
			{
				this->SetTriangle(idTriangle, { idVertex + 1, idVertex, beginId });
				//设置这个三角形的UV
				this->SetTriangleUVs(idTriangle,
					this->uvManager.getUV(idVertex + 1),
					this->uvManager.getUV(idVertex),
					this->uvManager.getUV(beginId)
				);
				//设置三角形的normal
				this->SetTriangleNormals(idTriangle,
					this->uvManager.getUV(idVertex + 1),
					this->uvManager.getUV(idVertex),
					this->uvManager.getUV(beginId)
				);
			}
			else
			{
				this->SetTriangle(idTriangle, { beginId, idVertex, idVertex + 1 });
				//设置三角形的UV
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
			//设置三角形所属的face
			this->SetTrianglePolygon(idTriangle, currFaceId);
			++idTriangle;
		}
	}

	//添加正方体对应的8个点
	void addBoxVertex()
	{
		//准备横截面的描述
		CrossSection crossSection;
		makeRectangleSection(crossSection);
		//在指定的Z轴位置添加这个截面
		addSection(0, crossSection);
		addSection(100, crossSection);
		//封闭指定的截面
		closeSection(0, crossSection, false);
		//封闭下截面
		closeSection(1, crossSection, true);
		//连接两个截面
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

	//生成一个基本的平面，这是为了检查自己对于单个三角面片的添加逻辑是否理解正确了
	void addTestTriangle()
	{
		//添加三个点
		this->ExtendBufferSizes(3, 1, 1, 1);
		//随便添加三个点
		this->SetVertex(0, Box.Frame.PointAt({ 0,0,0 }));
		this->SetVertex(1, Box.Frame.PointAt({ 0,500,0 }));
		this->SetVertex(2, Box.Frame.PointAt({ 500,0,0 }));
		//给三角形注册UV
		this->uvManager.beginRegisterUV();
		this->uvManager.registerUV(0);
		this->uvManager.registerUV(1);
		this->uvManager.registerUV(2);
		//结束对vertex的UV注册
		this->uvManager.endRegisterUV(*this);
		//设置三角形
		this->SetTriangle(0, 0, 1, 2);
		//设置UV
		this->SetTriangleUVs(0, uvManager.getUV(0),
			uvManager.getUV(1),
			uvManager.getUV(2)
		);
		this->SetTriangleNormals(0, uvManager.getUV(0),
			uvManager.getUV(1),
			uvManager.getUV(2)
		);
		//设置三角形所在的多边形
		this->SetTrianglePolygon(0, 0);
		//设置UV的渲染平面的位置
		this->UVs[uvManager.getUV(0)] = { 0,0 };
		this->UVs[uvManager.getUV(1)] = { 1,0 };
		this->UVs[uvManager.getUV(2)] = { 0,1 };
		//设置每个UV的法向量
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
	// 初始化好 box 之后，在这里打印 box里面的 extends
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
	//纹理的宽高
	constexpr uint32 texWidth = 800;
	constexpr uint32 texHeight = 800;
	//生成一个空的texture
	UTexture2D* retTexture = UTexture2D::CreateTransient(
		texWidth, texHeight, PF_B8G8R8A8
	);
	//对texture数据的基本配置
	TextureConfigLibrary::baseConfigTexture(*retTexture);
	//获取texture数据
	auto textureData = TextureConfigLibrary::getTextureData<uint8>(*retTexture);
	//给纹理数据添加云彩效果
	TextureEffect::PSCloudEffect(textureData, texWidth, texHeight,
		TextureEffect::Vec3(19, 11, 0), TextureEffect::Vec3(50, 41, 6));
	//给云彩添加杂色
	TextureEffect::addNoise(textureData, texWidth, texHeight,
		{ 200, 200, 200 }, 0.4
	);
	//对图片做动感模糊
	TextureEffect::motionBlur(textureData, texWidth, texHeight);
	//解锁texture数据
	TextureConfigLibrary::unlockTexture(*retTexture);
	//做texture数据的收尾工作
	TextureConfigLibrary::textureFinalUpdate(*retTexture);
	return retTexture;
}
