// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadFileLibrary.h"

#include "Runtime\Core\Public\Misc\FileHelper.h"
#include "Runtime\Core\Public\Misc\Paths.h"
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
