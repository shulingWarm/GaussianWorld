#pragma once

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

// 这里面放了3DGS加载的时候常用的一些函数
class GsSplatLib {
public:
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

	// 根据3DGS的个数，计算texture的大小
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

	// 从文件流里面读3DGS的数据
	//给颜色纹理数据赋值
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
};