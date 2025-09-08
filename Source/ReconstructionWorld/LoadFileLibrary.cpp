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

void ULoadFileLibrary::saveByteArrayAsFile(const char* saveData, uint32_t dataSize, std::string filePath)
{
    std::fstream fileHandle(filePath, std::ios::out | std::ios::binary);
    fileHandle.write(saveData, dataSize);
    fileHandle.close();
}

std::string ULoadFileLibrary::getFileSuffix(std::string filePath)
{
    // �����·��
    if (filePath.empty()) {
        return "";
    }

    // �������һ��·���ָ�����֧��Windows��Unix���
    size_t lastSeparator = filePath.find_last_of("/\\");
    size_t lastDot = filePath.find_last_of('.');

    // �����ļ�������·���ָ�����
    size_t fileNameStart = (lastSeparator == std::string::npos) ? 0 : lastSeparator + 1;
    std::string fileName = filePath.substr(fileNameStart);

    // ���������ļ�����"." �� ".."
    if (fileName == "." || fileName == "..") {
        return "";
    }

    // �����ļ����е����һ����
    lastDot = fileName.find_last_of('.');

    // û�е����ڿ�ͷ�������ļ�������ڽ�β����Ч��׺��
    if (lastDot == std::string::npos ||   // �޵�
        lastDot == 0 ||                   // �����ļ�����".gitignore"��
        lastDot == fileName.length() - 1) // �Ե��β����"file."��
    {
        return "";
    }

    // ��ȡ�����غ�׺��
    return fileName.substr(lastDot + 1);
}
