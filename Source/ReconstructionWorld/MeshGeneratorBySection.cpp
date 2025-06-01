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
	//添加节点位置的起始id
	uint32 beginId = this->Vertices.Num();
	//开辟节点的个数
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//遍历每个要添加的节点
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex)
	{
		auto& currPoint = section.vertexList[idVertex];
		SetVertex(beginId + idVertex, { currPoint.X, currPoint.Y, z });
	}
}

void MeshGeneratorBySection::addSection(CrossSection& section, FTransform3f& transform)
{
	//添加节点的起始位置id
	uint32 beginId = this->Vertices.Num();
	//开辟对应的节点个数
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//遍历每个要添加的节点
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex) {
		//转换当前位置的点
		FVector3f transformedPoint = transform.TransformPosition(
			VectorLibrary::convertVector<FVector, FVector3f>(section.vertexList[idVertex]));
		//添加转换后的节点
		SetVertex(beginId + idVertex,
			VectorLibrary::convertVector<FVector3f, FVector>(transformedPoint));
	}
}

void MeshGeneratorBySection::addSection(CrossSection& section, FVector3f center, FVector2i idMap)
{
	//添加节点的起始位置id
	uint32 beginId = this->Vertices.Num();
	//开辟对应的节点个数
	ExtendBufferSizes(section.vertexList.size(), 0, 0, 0);
	//遍历每个要添加的节点
	for (uint32 idVertex = 0; idVertex < section.vertexList.size(); ++idVertex) {
		FVector tempPoint = { 0,0,0 };
		auto& srcPoint = section.vertexList[idVertex];
		tempPoint[idMap[0]] = srcPoint.X;
		tempPoint[idMap[1]] = srcPoint.Y;
		//给节点添加偏移量
		tempPoint.X += center.X;
		tempPoint.Y += center.Y;
		tempPoint.Z += center.Z;
		//添加转换后的节点
		SetVertex(beginId + idVertex, tempPoint);
	}
}

void MeshGeneratorBySection::makeMeshBySection()
{
	//先添加section
	addSection(*sectionPtr, 0);
	addSection(*sectionPtr, meshHeight);
	//闭合下面的section
	closeSection(0, *sectionPtr, false);
	closeSection(1, *sectionPtr, true);
	//链接两个截面
	linkSection(*sectionPtr, 1);
}

void MeshGeneratorBySection::makeArchMesh()
{
	//弧形区段的半径
	float archRadius = 10.f;
	//直线部分的长度
	float straightLength = 100.f;
	//弧形区域的采样个数
	uint32 archSampleNum = 8;
	//计算弧度时每一步的角度
	float archStep = PI / 2 / archSampleNum;
	//弧形部分圆心所在的位置
	//假设弧形是在XOZ平面上的，并且最开始是沿着Z轴增长
	FVector3f archCenter(archRadius, 0, straightLength + archRadius);
	//添加垂直的直线区域
	addSection(*this->sectionPtr, 0);
	addSection(*this->sectionPtr, straightLength);
	//计算每一步时，mesh的中心点坐标
	FVector3f meshCenter;
	//添加弧形区域
	for (uint32 idArch = 1; idArch <= archSampleNum; ++idArch) {
		//当前点的角度
		float currentAngle = archStep * idArch;
		//当前的mesh中心点坐标
		float x = archRadius - archRadius * std::cos(currentAngle);
		float z = straightLength + archRadius * std::sin(currentAngle);
		//现在确认当前mesh的中心点
		meshCenter = { x,0,z };
		//计算当前mesh的旋转
		//pitch Y, Yaw Z, Roll, X
		//这个地方要把它转换成角度形式
		FRotator3f rotation((-currentAngle)/PI*180, 0, 0);
		//FRotator3f rotation(0, 0, 0);
		//新建旋转变换
		FTransform3f transform(rotation, meshCenter, FVector3f(1, 1, 1));
		//按照transform添加section
		this->addSection(*this->sectionPtr, transform);
	}
	//最后水平段的旋转
	FRotator3f horizontalRotation(-90, 0, 0);
	FTransform3f horizontalTrans(horizontalRotation,
		FVector3f(archRadius + straightLength, 0, archRadius + straightLength));
	this->addSection(*this->sectionPtr, horizontalTrans);
	this->addSection(*this->sectionPtr, { archRadius + straightLength, 0, archRadius + straightLength },
		{ 1,2 });
	//目前已经添加过的section个数
	uint32 sectionNum = this->Vertices.Num() / this->sectionPtr->vertexList.size();
	//到此为止仅仅是添加了节点，但还没有链接过节点
	for (uint32 idSection = 1; idSection < sectionNum; ++idSection) {
		linkSection(*this->sectionPtr, idSection);
	}
}

void MeshGeneratorBySection::closeSection(uint32 idSection, CrossSection& section, bool clockFlag)
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

uint32 MeshGeneratorBySection::getNewFaceId()
{
	++faceNum;
	return faceNum - 1;
}

void MeshGeneratorBySection::linkSection(CrossSection& section, int idSection)
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
		FVector3f tempNormal = VectorLibrary::getPointNormal<FVector3f>(
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

void MeshGeneratorBySection::addTriangleOnce(uint32 idTriangle, uint32 id0, uint32 id1, uint32 id2, uint32 faceId)
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

FMeshShapeGenerator& MeshGeneratorBySection::Generate()
{
	//根据section生成mesh
	if (generateMethod == CYLINDER)
		this->makeMeshBySection();
	else
		this->makeArchMesh();
	return *this;
}
