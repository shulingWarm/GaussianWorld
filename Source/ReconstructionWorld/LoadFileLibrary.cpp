// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadFileLibrary.h"

#include "Runtime\Core\Public\Misc\FileHelper.h"
#include "Runtime\Core\Public\Misc\Paths.h"
#include "Developer\DesktopPlatform\Public\IDesktopPlatform.h"
#include "Developer\DesktopPlatform\Public\DesktopPlatformModule.h"

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
