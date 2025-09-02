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

	// ��std stringת����FString
	static FString convertToFString(const std::string& oriString);

	// ��FStringת����std string
	static std::string convertToStdString(const FString& oriString);
};
