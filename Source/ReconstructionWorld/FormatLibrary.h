// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<string>
#include "CoreMinimal.h"

/**
 * 
 */
class RECONSTRUCTIONWORLD_API FormatLibrary
{
public:
	FormatLibrary();
	~FormatLibrary();

	// 把std string转换成FString
	static FString convertToFString(const std::string& oriString);

	// 把FString转换成std string
	static std::string convertToStdString(const FString& oriString);
};
