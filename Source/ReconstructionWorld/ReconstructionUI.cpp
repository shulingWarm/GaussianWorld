// Fill out your copyright notice in the Description page of Project Settings.


#include "ReconstructionUI.h"
#include <string>
#include"Vertex.h"
#include<vector>
#include<fstream>
#include<sstream>

//���ڴ����ؽ���������ݽṹ
struct SplatScene
{
	//�����
	uint32_t pointNum;
	//�ؽ��õ�3D��
	float* pointList;
	//��ɫ��Ϣ
	float* color;
	//scale��Ϣ
	float* scale;
	//��ת��Ϣ
	float* rotation;
	//͸����
	float* opacity;
};

//ply��ʽ�µ�����
class PlyProperty
{
public:
	std::string type;
	std::string name;
};

//ply���ļ���ʽ��Ϣ
class PlyHeader
{
public:
	std::string format;
	//�ڵ�ĸ���
	int verticeNum = 0;
	//��Ƭ�ĸ���
	int faceNum = 0;
	//���е������б������ڵ�ĺ���Ƭ��
	std::vector<PlyProperty> propertyList;

	//��ӡply header����Ϣ
	void print()
	{
		auto tempString = FString(format.c_str());
		UE_LOG(LogTemp, Warning, TEXT("format: %s\n"), *tempString);
		UE_LOG(LogTemp, Warning, TEXT("vertice num: %d\n"), verticeNum);
		UE_LOG(LogTemp, Warning, TEXT("face num: %d\n"), faceNum);
	}
};

//�ؽ����̵�ִ����
class ReconstructionRunner : public FRunnable
{
public:
	//ʵ�ʱ����õĺ���ָ��
	AReconstructionUI::ReconstructionFunc funcPtr;
	//�����ؽ���ͼƬ·���͹���·��
	std::string imagePath;
	std::string workspacePath;
	//�ؽ���ɺ�Ļص�
	AReconstructionUI* clientCallback;

	//���캯������Ҫ�������ڹ���ĺ���
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
		//׼�����ڽ����ؽ�����Ľṹ
		SplatScene* tempResult = new SplatScene();
		//ֱ�ӵ��������ؽ��ĺ���
		this->funcPtr(imagePath.c_str(), workspacePath.c_str(),
			reinterpret_cast<void*>(tempResult));
		return 0;
	}
};

//����dll��ȡ����ָ��
static AReconstructionUI::ReconstructionFunc loadReconstructionFunction()
{
	DWORD prev = SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	//����dll������·��
	SetDefaultDllDirectories(prev | LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
		LOAD_LIBRARY_SEARCH_USER_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
	AddDllDirectory(TEXT("E:/privateSpace/workSpace/504OpenSplat/OpenSplat-main/OpenSplat-main/build/Release"));
	AddDllDirectory(TEXT("E:/lib/vcpkg/installed/x64-windows/bin"));
	AddDllDirectory(TEXT("E:/lib/libtorch/libtorch/lib"));
	AddDllDirectory(TEXT("E:/lib/cuda-win/bin"));
	//�����������ݵ�ģ��
	//����ʵҲ��һ��ָ�룬ֻ��������ģ��ָ��
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
	//�������ȡִ���ؽ��ĺ���ָ��
	this->reconstruction_func = loadReconstructionFunction();
}

//ִ���ؽ���������
void AReconstructionUI::beginReconstruction()
{
	//�½�һ���ؽ���ִ����
	//������ö�̬����Ķ���
	auto tempRunner = new ReconstructionRunner(this->reconstruction_func,
		"E:/temp/south-building/images", "E:/temp/tempWorkspace", this);
	//�����߳�����ʼִ��
	FRunnableThread* tempThread = FRunnableThread::Create(tempRunner, TEXT("Reconstruction task"));
}

//������Ƶ�����ͷ
static void loadPlyHeader(std::ifstream& fileHandle,
	PlyHeader& header
)
{
	//�м��ȡ����ÿһ�еĽ��
	std::string line;
	while (std::getline(fileHandle, line))
	{
		//�½�������
		std::istringstream tempStream(line);
		//��ȡһ������
		std::string token;
		tempStream >> token;
		//�ж��ǲ��Ǹ�ʽ��Ϣ
		if (token == "format")
		{
			tempStream >> header.format;
		}
		//�ж϶�ȡ�����ǲ���element��Ϣ
		else if (token == "element")
		{
			//�ٶ�ȡelement������
			tempStream >> token;
			//�ж��ǽڵ����������Ƭ�ĸ���
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
		//���ж��Ƿ��ȡ����������Ϣ
		else if (token == "property")
		{
			//�½�һ����ʱ������
			PlyProperty tempProperty;
			//��¼����type������
			tempStream >> tempProperty.type >> tempProperty.name;
			//�����Էŵ��б�����
			header.propertyList.push_back(tempProperty);
		}
		//header���ֵĽ�����
		else if (token == "end_header")
		{
			break;
		}

	}
}

//����һ��ply�ļ�
//������ǲ����õĺ���
static void loadPointcloud(std::vector<Vertex>& vertexList,std::string filePath)
{
	//���ļ���������
	std::ifstream fileHandle(filePath, std::ios::binary);
	//�½�ply��ͷ��������
	PlyHeader header;
	loadPlyHeader(fileHandle, header);
	UE_LOG(LogTemp, Warning, TEXT("Read ply header ok"));
	//����֮���ӡһ��header��Ϣ
	header.print();
	//������ȡÿ����
	std::vector<Vertex> tempVertex(header.verticeNum);
	if (sizeof(Vertex) != 62 * sizeof(float))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid vertex size"));
		throw std::runtime_error("Invalid vertex size");
	}
	int idData = 0;
	for (auto& eachVertex : tempVertex)
	{
		//��ȡ�ڵ�����
		fileHandle.read(reinterpret_cast<char*>(&eachVertex), sizeof(Vertex));
		//��ӡ62������
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
	//����ÿ���ڵ�
	vertexList.reserve(tempVertex.size());
	for (auto& eachVertex : tempVertex)
	{
		//�ж��Ƿ��ڰ뾶��
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

//��ȡ����Ŀ�Ⱥ͸߶�
static FInt32Vector2 getTextureSize(int gaussianNum)
{
	FInt32Vector2 ret;
	//xĬ����2048
	ret.X = 2048;
	//Y ȡ ����ȡ���Ķ�����������
	ret.Y = FMath::RoundUpToPowerOfTwo((float)gaussianNum / ret.X);
	return ret;
}

//��ȡ�������ɫ����
template<class DataType>
static DataType* getTextureData(UTexture2D& texture)
{
	//��ȡ�����ƽ̨����
	auto& platformData = *texture.GetPlatformData();
	//��ȡplatform�����mip����
	auto& mip = platformData.Mips.Last();
	//������������
	return reinterpret_cast<DataType*>(mip.BulkData.Lock(LOCK_READ_WRITE));
}

//��ɫ���ݵ�ӳ���ϵ
static float colorAdjust(float srcColor)
{
	return srcColor * 0.2820948f + 0.5f;
}

//��͸���ȵĵ���
static float adjustOpacity(float opacity)
{
	return 1.f / (1 + std::exp(-opacity));
}

//��scale���ݵĵ���
static float sizeAdjust(float scale)
{
	return std::exp(scale);
}

//����ת��Ϣ�Ĵ���
static FVector4f rotAdjust(float* src)
{
	auto srcRotation = FVector4f(src[0], src[1], src[2], src[3]);
	//����֮�������
	auto vecLength = srcRotation.Size();
	if (vecLength > 0)
		return srcRotation / vecLength;
	return srcRotation;
}

//����ɫ�������ݸ�ֵ
//Ū��������ʽ��Ϊ�˸�������ϵͳ������������
static void assignTextureData(
	std::ifstream& fileHandle, //���ڶ�ȡ���ı�������
	FFloat16* posData, //λ������
	FFloat16* colorData, //��ɫ����
	FFloat16* sizeData, //ÿ����˹���size������Ϣ
	FFloat16* rotData, //ÿ����˹�����ת������Ϣ
	int pointNum,
	int pixelNum //�����������
)
{

	//���ڶ�ȡ�����������ݴ�С��Ŀǰ��ʱ��������չ��
	const int BUFFER_SIZE = 62;
	//ÿ���������������Ӧ�����ݿ�
	const int TEX_BLOCK_SIZE = 4;
	//posλ�õ����ţ�Ϊ�˼�С�������
	const float POS_SCALE = 1.f / 32;
	//���ڶ�ȡ���ݵĻ�����
	float pointBuffer[BUFFER_SIZE];
	//������ȡÿ����
	for (int idPoint = 0; idPoint < pointNum; ++idPoint)
	{
		//��ǰ��pos��color������ͷ
		auto posHead = posData + idPoint * TEX_BLOCK_SIZE;
		auto colorHead = colorData + idPoint * TEX_BLOCK_SIZE;
		auto sizeHead = sizeData + idPoint * TEX_BLOCK_SIZE;
		auto rotHead = rotData + idPoint * TEX_BLOCK_SIZE;
		//��ʱ��ȡ����
		fileHandle.read((char*)pointBuffer, sizeof(float) * BUFFER_SIZE);
		//�ж��Ƿ���Ҫ��ӡ����
		//if (idPoint < 3)
		//{
		//	printDataInBuffer(pointBuffer);
		//}
		//��¼��������
		posHead[0] = FFloat16(pointBuffer[0] * POS_SCALE);
		posHead[1] = FFloat16(-pointBuffer[2] * POS_SCALE);
		posHead[2] = FFloat16(-pointBuffer[1] * POS_SCALE);
		for (int i = 0; i < 3; ++i) colorHead[i] = FFloat16(
			colorAdjust(pointBuffer[6 + i]));
		//��������ɫ�����һ��ͨ���ĳɹ̶�ֵ1
		posHead[3] = FFloat16(0);
		//��ɫ����ֵΪ͸����
		colorHead[3] = FFloat16(adjustOpacity(pointBuffer[54]));
		for (int i = 0; i < 3; ++i)
		{
			//������¼size��Ϣ
			sizeHead[i] = FFloat16(sizeAdjust(pointBuffer[55 + i]) * POS_SCALE);
		}
		//��ȡ���������ת����
		auto adjustedRot = rotAdjust(&pointBuffer[58]);
		//������ת��Ϣ
		for (int i = 0; i < 4; ++i)
		{
			rotHead[i] = FFloat16(adjustedRot[i]);
		}
	}
	//��ӡ2200��ʼλ�õ�10����
	std::stringstream sstream;
	for (int i = 0; i < 10; ++i)
	{
		sstream << (float)sizeData[2200 + i] << " ";
	}
	UE_LOG(LogTemp, Warning, TEXT("PosData: %s\n"), *FString(sstream.str().c_str()));
	//�ж������Ƿ���ʣ����ɫ
	for (int idPoint = pointNum; idPoint < pixelNum; ++idPoint)
	{
		auto posHead = posData + idPoint * TEX_BLOCK_SIZE;
		auto colorHead = colorData + idPoint * TEX_BLOCK_SIZE;
		for (int i = 0; i < 4; ++i) posHead[i] = FFloat16(0);
		for (int i = 0; i < 4; ++i) colorHead[i] = FFloat16(0);
	}
}

//�������ڶ�ȡ���ݵ�texture
static void unlockTexture(UTexture2D& texture)
{
	texture.GetPlatformData()->Mips.Last().BulkData.Unlock();
}

#undef UpdateResource

//��texture������֮�������ĸ���
static void textureFinalUpdate(UTexture2D& texture)
{
	texture.UpdateResource();
	texture.AddToRoot();
}

#define UpdateResource UpdateResourceW

void baseConfigTexture(UTexture2D& texture)
{
	//������ʽ�ĳ��������
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
	//C++��ʽ�ĵ��ƶ�ȡ
	std::string stdPlyPath(TCHAR_TO_UTF8(*plyPath));
	std::ifstream fileHandle(stdPlyPath,std::ios::in|std::ios::binary);
	//��ȡply������ͷ
	PlyHeader header;
	loadPlyHeader(fileHandle, header);
	//���ݵ������ȡtexture�Ĵ�С
	auto textureSize = getTextureSize(header.verticeNum);
	//ָ���ڵ����
	descriptor->gaussianNum = header.verticeNum;
	//��ȡtexture���������
	FFloat16* textureData[4];
	//��ʼ��4��texture
	for(int idTexture=0;idTexture<4;++idTexture)
	{
		//��ʼ��һ����ʱ��texture
		auto tempTexture = UTexture2D::CreateTransient(
			textureSize.X, textureSize.Y, PF_FloatRGBA
		);
		//���������ݵĻ�������
		baseConfigTexture(*tempTexture);
		//��¼texture����
		textureData[idTexture] = getTextureData<FFloat16>(*tempTexture);
		//��ӵ�texture���б�����
		retTexture.Add(tempTexture);
	}
	//�����ݶ�ȡ��texture����
	assignTextureData(fileHandle,textureData[0],textureData[1],textureData[2],textureData[3],
		header.verticeNum,retTexture[0]->GetSizeX()* retTexture[0]->GetSizeY());
	//����texture���ݣ���������
	for(int idTexture=0;idTexture<4;++idTexture)
	{
		unlockTexture(retTexture[idTexture][0]);
		textureFinalUpdate(retTexture[idTexture][0]);
	}
	//ָ��descriptor�Ĳ�����С
	descriptor->sampleSizeX = retTexture[0]->GetSizeX();
	descriptor->sampleSizeY = descriptor->gaussianNum / descriptor->sampleSizeX + 1;
	//ָ����������
	descriptor->sampleStep[0] = 1.f / (retTexture[0]->GetSizeX() + 0.5f);
	descriptor->sampleStep[1] = 1.f / (retTexture[0]->GetSizeY());
	//��ʼ��һ���ڵ��б���������ֵ
	std::vector<Vertex> vertexList;
	loadPointcloud(vertexList,std::string(TCHAR_TO_UTF8(*plyPath)));
	//��ӡ�����
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

