// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadFileLibrary.h"

#include "Runtime\Core\Public\Misc\FileHelper.h"
#include "Runtime\Core\Public\Misc\Paths.h"
#include<fstream>
#include "Developer\DesktopPlatform\Public\IDesktopPlatform.h"
#include "Developer\DesktopPlatform\Public\DesktopPlatformModule.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"

FString ULoadFileLibrary::getImageFolder()
{
	TArray<FString> OpenFileNames;//获取的文件绝对路径
	FString ExtensionStr = TEXT("*");//文件类型

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString retFolder;
	DesktopPlatform->OpenDirectoryDialog(
		nullptr, TEXT("选择图片"), "", retFolder);
	UE_LOG(LogTemp,Warning,TEXT("%s\n"),*retFolder);
	return retFolder;
}

FString ULoadFileLibrary::getSpecificFile(FString extension)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	TArray<FString> selectedFiles;
	int32 file_num;
	FString retFile;
	DesktopPlatform->OpenFileDialog(nullptr,TEXT("选择图片"),TEXT(""),TEXT(""),extension,EFileDialogFlags::None,selectedFiles,file_num);
	return selectedFiles[0];
}

// 用来遍历文件的工具
// 遍历目录
class FFileVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
    TArray<FString>& FileNames;
    FFileVisitor(TArray<FString>& InFiles) : FileNames(InFiles) {}

    virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
    {
        if (!bIsDirectory) // 仅添加文件
        {
            FileNames.Add(FPaths::GetCleanFilename(FilenameOrDirectory));
        }
        return true;
    }
};

void ULoadFileLibrary::GetFilesInDirectory(const FString& Directory, TArray<FString>& FileNames)
{
    // 获取平台文件接口
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // 检查目录存在
    if (!PlatformFile.DirectoryExists(*Directory))
    {
        UE_LOG(LogTemp, Warning, TEXT("Directory not found: %s"), *Directory);
    }

    FFileVisitor Visitor(FileNames);
    PlatformFile.IterateDirectory(*Directory, Visitor);
}

void ULoadFileLibrary::saveByteArrayAsFile(const char* saveData, uint32_t dataSize, std::string filePath)
{
    std::fstream fileHandle(filePath, std::ios::out | std::ios::binary);
    fileHandle.write(saveData, dataSize);
    fileHandle.close();
}

std::string ULoadFileLibrary::getFileSuffix(std::string filePath)
{
    // 处理空路径
    if (filePath.empty()) {
        return "";
    }

    // 查找最后一个路径分隔符（支持Windows和Unix风格）
    size_t lastSeparator = filePath.find_last_of("/\\");
    size_t lastDot = filePath.find_last_of('.');

    // 处理纯文件名（无路径分隔符）
    size_t fileNameStart = (lastSeparator == std::string::npos) ? 0 : lastSeparator + 1;
    std::string fileName = filePath.substr(fileNameStart);

    // 处理特殊文件名："." 和 ".."
    if (fileName == "." || fileName == "..") {
        return "";
    }

    // 查找文件名中的最后一个点
    lastDot = fileName.find_last_of('.');

    // 没有点或点在开头（隐藏文件）或点在结尾（无效后缀）
    if (lastDot == std::string::npos ||   // 无点
        lastDot == 0 ||                   // 隐藏文件（如".gitignore"）
        lastDot == fileName.length() - 1) // 以点结尾（如"file."）
    {
        return "";
    }

    // 提取并返回后缀名
    return fileName.substr(lastDot + 1);
}
