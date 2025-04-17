// Fill out your copyright notice in the Description page of Project Settings.


#include "ReconstructionUI.h"
#include <string>
#include"Vertex.h"
#include<vector>
#include<fstream>
#include<sstream>

//用于传输重建结果的数据结构
struct SplatScene
{
	//点个数
	uint32_t pointNum;
	//重建好的3D点
	float* pointList;
	//颜色信息
	float* color;
	//scale信息
	float* scale;
	//旋转信息
	float* rotation;
	//透明度
	float* opacity;
};

//ply格式下的属性
class PlyProperty
{
public:
	std::string type;
	std::string name;
};

//ply的文件格式信息
class PlyHeader
{
public:
	std::string format;
	//节点的个数
	int verticeNum = 0;
	//面片的个数
	int faceNum = 0;
	//所有的属性列表，包括节点的和面片的
	std::vector<PlyProperty> propertyList;

	//打印ply header的信息
	void print()
	{
		auto tempString = FString(format.c_str());
		UE_LOG(LogTemp, Warning, TEXT("format: %s\n"), *tempString);
		UE_LOG(LogTemp, Warning, TEXT("vertice num: %d\n"), verticeNum);
		UE_LOG(LogTemp, Warning, TEXT("face num: %d\n"), faceNum);
	}
};

//重建过程的执行器
class ReconstructionRunner : public FRunnable
{
public:
	//实际被调用的函数指针
	AReconstructionUI::ReconstructionFunc funcPtr;
	//用于重建的图片路径和工作路径
	std::string imagePath;
	std::string workspacePath;
	//重建完成后的回调
	AReconstructionUI* clientCallback;

	//构造函数，需要传入用于构造的函数
	ReconstructionRunner(AReconstructionUI::ReconstructionFunc funcPtr,
		std::string imagePath,
		std::string workspacePath,
		AReconstructionUI* clientCallback
	)
	{
		this->funcPtr = funcPtr;
		this->clientCallback = clientCallback;
		this->imagePath = imagePath;
		this->workspacePath = workspacePath;
	}

	virtual uint32 Run() override
	{
		UE_LOG(LogTemp,Warning,TEXT("Begin run reconstruction"));
		//准备用于接收重建结果的结构
		SplatScene* tempResult = new SplatScene();
		//直接调用用于重建的函数
		this->funcPtr(imagePath.c_str(), workspacePath.c_str(),
			reinterpret_cast<void*>(tempResult));
		return 0;
	}
};

//加载dll获取函数指针
static AReconstructionUI::ReconstructionFunc loadReconstructionFunction()
{
	DWORD prev = SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	//设置dll的搜索路径
	SetDefaultDllDirectories(prev | LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
		LOAD_LIBRARY_SEARCH_USER_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
	AddDllDirectory(TEXT("E:/privateSpace/workSpace/504OpenSplat/OpenSplat-main/OpenSplat-main/build/Release"));
	AddDllDirectory(TEXT("E:/lib/vcpkg/installed/x64-windows/bin"));
	AddDllDirectory(TEXT("E:/lib/libtorch/libtorch/lib"));
	AddDllDirectory(TEXT("E:/lib/cuda-win/bin"));
	//包含函数内容的模块
	//这其实也是一个指针，只不过这是模块指针
	HMODULE hModule;
	hModule = LoadLibrary(TEXT("E:/privateSpace/workSpace/504OpenSplat/OpenSplat-main/OpenSplat-main/build/Release/IntegralReconstruction.dll"));
	if (hModule == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Cannot load module"));
		return nullptr;
	}
	AReconstructionUI::ReconstructionFunc reconstructionFunc;
	reconstructionFunc = (AReconstructionUI::ReconstructionFunc)GetProcAddress(hModule,"reconstruct");
	if (reconstructionFunc == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find function"));
		return nullptr;
	}
	return reconstructionFunc;
}

// Sets default values
AReconstructionUI::AReconstructionUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//在这里获取执行重建的函数指针
	this->reconstruction_func = loadReconstructionFunction();
}

//执行重建的主流程
void AReconstructionUI::beginReconstruction()
{
	//新建一个重建的执行器
	//这必须用动态构造的对象
	auto tempRunner = new ReconstructionRunner(this->reconstruction_func,
		"E:/temp/south-building/images", "E:/temp/tempWorkspace", this);
	//构造线程来开始执行
	FRunnableThread* tempThread = FRunnableThread::Create(tempRunner, TEXT("Reconstruction task"));
}

//载入点云的数据头
static void loadPlyHeader(std::ifstream& fileHandle,
	PlyHeader& header
)
{
	//中间读取到的每一行的结果
	std::string line;
	while (std::getline(fileHandle, line))
	{
		//新建输入流
		std::istringstream tempStream(line);
		//读取一个单词
		std::string token;
		tempStream >> token;
		//判断是不是格式信息
		if (token == "format")
		{
			tempStream >> header.format;
		}
		//判断读取到的是不是element信息
		else if (token == "element")
		{
			//再读取element的类型
			tempStream >> token;
			//判断是节点个数还是面片的个数
			if (token == "vertex")
			{
				tempStream >> header.verticeNum;
			}
			else if (token == "face")
			{
				tempStream >> header.faceNum;
			}
			else
			{
				throw std::runtime_error("Unknown element type");
			}
		}
		//再判断是否读取到了属性信息
		else if (token == "property")
		{
			//新建一个临时的属性
			PlyProperty tempProperty;
			//记录它的type和名字
			tempStream >> tempProperty.type >> tempProperty.name;
			//把属性放到列表里面
			header.propertyList.push_back(tempProperty);
		}
		//header部分的结束符
		else if (token == "end_header")
		{
			break;
		}

	}
}

//载入一个ply文件
//这仅仅是测试用的函数
static void loadPointcloud(std::vector<Vertex>& vertexList,std::string filePath)
{
	//打开文件的输入流
	std::ifstream fileHandle(filePath, std::ios::binary);
	//新建ply的头部描述符
	PlyHeader header;
	loadPlyHeader(fileHandle, header);
	UE_LOG(LogTemp, Warning, TEXT("Read ply header ok"));
	//读完之后打印一下header信息
	header.print();
	//遍历读取每个点
	std::vector<Vertex> tempVertex(header.verticeNum);
	if (sizeof(Vertex) != 62 * sizeof(float))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid vertex size"));
		throw std::runtime_error("Invalid vertex size");
	}
	int idData = 0;
	for (auto& eachVertex : tempVertex)
	{
		//读取节点数据
		fileHandle.read(reinterpret_cast<char*>(&eachVertex), sizeof(Vertex));
		//打印62个数据
		float* vertexData = (float*)&eachVertex;
		if (idData < 20)
		{
			std::stringstream tempStream;
			for (int i = 0; i < 62; ++i)
			{
				tempStream << vertexData[i] << " ";
			}
			UE_LOG(LogTemp, Warning, TEXT("%s\n"), *FString(tempStream.str().c_str()));
		}
		++idData;
	}
	//遍历每个节点
	vertexList.reserve(tempVertex.size());
	for (auto& eachVertex : tempVertex)
	{
		//判断是否在半径内
		float sum = 0;
		for (int i = 0; i < 3; ++i)
		{
			auto tempVal = eachVertex.position[i];
			sum += tempVal * tempVal;
		}
		if (sum < 81)
		{
			vertexList.push_back(eachVertex);
		}
	}
}

//获取纹理的宽度和高度
static FInt32Vector2 getTextureSize(int gaussianNum)
{
	FInt32Vector2 ret;
	//x默认是2048
	ret.X = 2048;
	//Y 取 向上取整的二的整数次幂
	ret.Y = FMath::RoundUpToPowerOfTwo((float)gaussianNum / ret.X);
	return ret;
}

//获取纹理的颜色数据
template<class DataType>
static DataType* getTextureData(UTexture2D& texture)
{
	//获取纹理的平台数据
	auto& platformData = *texture.GetPlatformData();
	//获取platform里面的mip数据
	auto& mip = platformData.Mips.Last();
	//锁定主体数据
	return reinterpret_cast<DataType*>(mip.BulkData.Lock(LOCK_READ_WRITE));
}

//颜色数据的映射关系
static float colorAdjust(float srcColor)
{
	return srcColor * 0.2820948f + 0.5f;
}

//对透明度的调整
static float adjustOpacity(float opacity)
{
	return 1.f / (1 + std::exp(-opacity));
}

//对scale数据的调整
static float sizeAdjust(float scale)
{
	return std::exp(scale);
}

//对旋转信息的处理
static FVector4f rotAdjust(float* src)
{
	auto srcRotation = FVector4f(src[0], src[1], src[2], src[3]);
	//正则化之后的向量
	auto vecLength = srcRotation.Size();
	if (vecLength > 0)
		return srcRotation / vecLength;
	return srcRotation;
}

//给颜色纹理数据赋值
//弄成这种形式是为了复用粒子系统里面的纹理采样
static void assignTextureData(
	std::ifstream& fileHandle, //正在读取的文本输入流
	FFloat16* posData, //位置数据
	FFloat16* colorData, //颜色数据
	FFloat16* sizeData, //每个高斯点的size数据信息
	FFloat16* rotData, //每个高斯点的旋转数据信息
	int pointNum,
	int pixelNum //纹理的像素数
)
{

	//用于读取缓冲区的数据大小，目前暂时不考虑扩展性
	const int BUFFER_SIZE = 62;
	//每个点在纹理里面对应的数据块
	const int TEX_BLOCK_SIZE = 4;
	//pos位置的缩放，为了减小量化误差
	const float POS_SCALE = 1.f / 32;
	//用于读取数据的缓冲区
	float pointBuffer[BUFFER_SIZE];
	//遍历读取每个点
	for (int idPoint = 0; idPoint < pointNum; ++idPoint)
	{
		//当前的pos和color的数据头
		auto posHead = posData + idPoint * TEX_BLOCK_SIZE;
		auto colorHead = colorData + idPoint * TEX_BLOCK_SIZE;
		auto sizeHead = sizeData + idPoint * TEX_BLOCK_SIZE;
		auto rotHead = rotData + idPoint * TEX_BLOCK_SIZE;
		//临时读取数据
		fileHandle.read((char*)pointBuffer, sizeof(float) * BUFFER_SIZE);
		//判断是否需要打印数据
		//if (idPoint < 3)
		//{
		//	printDataInBuffer(pointBuffer);
		//}
		//记录坐标数据
		posHead[0] = FFloat16(pointBuffer[0] * POS_SCALE);
		posHead[1] = FFloat16(-pointBuffer[2] * POS_SCALE);
		posHead[2] = FFloat16(-pointBuffer[1] * POS_SCALE);
		for (int i = 0; i < 3; ++i) colorHead[i] = FFloat16(
			colorAdjust(pointBuffer[6 + i]));
		//把两个颜色的最后一个通道改成固定值1
		posHead[3] = FFloat16(0);
		//颜色被赋值为透明度
		colorHead[3] = FFloat16(adjustOpacity(pointBuffer[54]));
		for (int i = 0; i < 3; ++i)
		{
			//遍历记录size信息
			sizeHead[i] = FFloat16(sizeAdjust(pointBuffer[55 + i]) * POS_SCALE);
		}
		//获取处理过的旋转数据
		auto adjustedRot = rotAdjust(&pointBuffer[58]);
		//保存旋转信息
		for (int i = 0; i < 4; ++i)
		{
			rotHead[i] = FFloat16(adjustedRot[i]);
		}
	}
	//打印2200开始位置的10个数
	std::stringstream sstream;
	for (int i = 0; i < 10; ++i)
	{
		sstream << (float)sizeData[2200 + i] << " ";
	}
	UE_LOG(LogTemp, Warning, TEXT("PosData: %s\n"), *FString(sstream.str().c_str()));
	//判断纹理是否有剩余颜色
	for (int idPoint = pointNum; idPoint < pixelNum; ++idPoint)
	{
		auto posHead = posData + idPoint * TEX_BLOCK_SIZE;
		auto colorHead = colorData + idPoint * TEX_BLOCK_SIZE;
		for (int i = 0; i < 4; ++i) posHead[i] = FFloat16(0);
		for (int i = 0; i < 4; ++i) colorHead[i] = FFloat16(0);
	}
}

//解锁正在读取数据的texture
static void unlockTexture(UTexture2D& texture)
{
	texture.GetPlatformData()->Mips.Last().BulkData.Unlock();
}

#undef UpdateResource

//给texture处理完之后，做最后的更新
static void textureFinalUpdate(UTexture2D& texture)
{
	texture.UpdateResource();
	texture.AddToRoot();
}

#define UpdateResource UpdateResourceW

void baseConfigTexture(UTexture2D& texture)
{
	//采样方式改成最近采样
	texture.Filter = TextureFilter::TF_Nearest;
	texture.CompressionSettings = TextureCompressionSettings::TC_HDR;
	texture.SRGB = false;
	texture.NeverStream = true;
	auto platformData = texture.GetPlatformData();
	platformData->SetNumSlices(1);
}

void AReconstructionUI::cppLoadPlyFile(FString plyPath, UGaussianDescriptor* descriptor)
{
	TArray<UTexture2D*>& retTexture = descriptor->textreList;
	//C++形式的点云读取
	std::string stdPlyPath(TCHAR_TO_UTF8(*plyPath));
	std::ifstream fileHandle(stdPlyPath,std::ios::in|std::ios::binary);
	//读取ply的数据头
	PlyHeader header;
	loadPlyHeader(fileHandle, header);
	//根据点个数获取texture的大小
	auto textureSize = getTextureSize(header.verticeNum);
	//指定节点个数
	descriptor->gaussianNum = header.verticeNum;
	//读取texture里面的数据
	FFloat16* textureData[4];
	//初始化4个texture
	for(int idTexture=0;idTexture<4;++idTexture)
	{
		//初始化一个临时的texture
		auto tempTexture = UTexture2D::CreateTransient(
			textureSize.X, textureSize.Y, PF_FloatRGBA
		);
		//对纹理数据的基础配置
		baseConfigTexture(*tempTexture);
		//记录texture数据
		textureData[idTexture] = getTextureData<FFloat16>(*tempTexture);
		//添加到texture的列表里面
		retTexture.Add(tempTexture);
	}
	//把数据读取进texture里面
	assignTextureData(fileHandle,textureData[0],textureData[1],textureData[2],textureData[3],
		header.verticeNum,retTexture[0]->GetSizeX()* retTexture[0]->GetSizeY());
	//遍历texture数据，解锁数据
	for(int idTexture=0;idTexture<4;++idTexture)
	{
		unlockTexture(retTexture[idTexture][0]);
		textureFinalUpdate(retTexture[idTexture][0]);
	}
	//指定descriptor的采样大小
	descriptor->sampleSizeX = retTexture[0]->GetSizeX();
	descriptor->sampleSizeY = descriptor->gaussianNum / descriptor->sampleSizeX + 1;
	//指定采样步长
	descriptor->sampleStep[0] = 1.f / (retTexture[0]->GetSizeX() + 0.5f);
	descriptor->sampleStep[1] = 1.f / (retTexture[0]->GetSizeY());
	//初始化一个节点列表用作返回值
	std::vector<Vertex> vertexList;
	loadPointcloud(vertexList,std::string(TCHAR_TO_UTF8(*plyPath)));
	//打印点个数
	UE_LOG(LogTemp,Warning,TEXT("Splat num: %d\n"),vertexList.size());
}

// Called when the game starts or when spawned
void AReconstructionUI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReconstructionUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

