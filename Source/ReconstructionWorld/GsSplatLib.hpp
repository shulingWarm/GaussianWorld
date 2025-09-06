#pragma once

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

// ���������3DGS���ص�ʱ���õ�һЩ����
class GsSplatLib {
public:
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

	// ����3DGS�ĸ���������texture�Ĵ�С
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

	// ���ļ��������3DGS������
	//����ɫ�������ݸ�ֵ
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
};