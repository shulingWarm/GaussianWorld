#pragma once
#include<string>
#include<vector>
#include"LoadFileLibrary.h"
#include"FormatLibrary.h"

// ͼƬ�б�
// ���ͼƬ�б�������δ���ص�ͼƬ�б�
class ImageList {
public:
	// ����ͼƬ��Ϣ��·���б�
	std::vector<std::string> imagePathList;

	// ʹ�ð���ͼƬ���ļ�������������
	ImageList(std::string imageFolder) {
		// ת����FString
		FString fStringFolder = FormatLibrary::convertToFString(imageFolder);
		// ��ȡ�ļ����������е��ļ�
		TArray<FString> tempFileList;
		ULoadFileLibrary::GetFilesInDirectory(fStringFolder, tempFileList);
		imagePathList.reserve(tempFileList.Num());
		uint32_t listLength = tempFileList.Num();
		// ��ʱ������׺����������Ժ���˵
		// ��FStringת����std::string
		for (uint32_t idImage = 0; idImage < listLength; ++idImage) {
			// ƴ�ӳɾ���·��
			auto absPath = imageFolder + "/" + FormatLibrary::convertToStdString(tempFileList[idImage]);
			imagePathList.push_back(absPath);
		}
	}

	// ��ȡͼƬ�ĸ���
	uint32_t getImageNum() {
		return this->imagePathList.size();
	}
};