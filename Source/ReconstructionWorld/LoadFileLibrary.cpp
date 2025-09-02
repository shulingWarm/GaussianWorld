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
	TArray<FString> OpenFileNames;//��ȡ���ļ�����·��
	FString ExtensionStr = TEXT("*");//�ļ�����

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString retFolder;
	DesktopPlatform->OpenDirectoryDialog(
		nullptr, TEXT("ѡ��ͼƬ"), "", retFolder);
	UE_LOG(LogTemp,Warning,TEXT("%s\n"),*retFolder);
	return retFolder;
}

FString ULoadFileLibrary::getSpecificFile(FString extension)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	TArray<FString> selectedFiles;
	int32 file_num;
	FString retFile;
	DesktopPlatform->OpenFileDialog(nullptr,TEXT("ѡ��ͼƬ"),TEXT(""),TEXT(""),extension,EFileDialogFlags::None,selectedFiles,file_num);
	return selectedFiles[0];
}

// ���������ļ��Ĺ���
// ����Ŀ¼
class FFileVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
    TArray<FString>& FileNames;
    FFileVisitor(TArray<FString>& InFiles) : FileNames(InFiles) {}

    virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
    {
        if (!bIsDirectory) // ������ļ�
        {
            FileNames.Add(FPaths::GetCleanFilename(FilenameOrDirectory));
        }
        return true;
    }
};

void ULoadFileLibrary::GetFilesInDirectory(const FString& Directory, TArray<FString>& FileNames)
{
    // ��ȡƽ̨�ļ��ӿ�
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // ���Ŀ¼����
    if (!PlatformFile.DirectoryExists(*Directory))
    {
        UE_LOG(LogTemp, Warning, TEXT("Directory not found: %s"), *Directory);
    }

    FFileVisitor Visitor(FileNames);
    PlatformFile.IterateDirectory(*Directory, Visitor);
}
