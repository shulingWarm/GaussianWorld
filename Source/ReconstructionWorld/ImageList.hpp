#pragma once
#include<string>
#include<vector>
#include"LoadFileLibrary.h"
#include"FormatLibrary.h"

// 图片列表
// 这个图片列表是那种未加载的图片列表
class ImageList {
public:
	// 包含图片信息的路径列表
	std::vector<std::string> imagePathList;

	// 使用包含图片的文件夹来当作输入
	ImageList(std::string imageFolder) {
		// 转换成FString
		FString fStringFolder = FormatLibrary::convertToFString(imageFolder);
		// 获取文件夹里面所有的文件
		TArray<FString> tempFileList;
		ULoadFileLibrary::GetFilesInDirectory(fStringFolder, tempFileList);
		imagePathList.reserve(tempFileList.Num());
		uint32_t listLength = tempFileList.Num();
		// 暂时不检查后缀名，这个事以后再说
		// 把FString转换成std::string
		for (uint32_t idImage = 0; idImage < listLength; ++idImage) {
			// 拼接成绝对路径
			auto absPath = imageFolder + "/" + FormatLibrary::convertToStdString(tempFileList[idImage]);
			imagePathList.push_back(absPath);
		}
	}

	// 获取图片的个数
	uint32_t getImageNum() {
		return this->imagePathList.size();
	}
};