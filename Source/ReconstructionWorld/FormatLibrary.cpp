// Fill out your copyright notice in the Description page of Project Settings.


#include "FormatLibrary.h"

FormatLibrary::FormatLibrary()
{

}

FormatLibrary::~FormatLibrary()
{

}

FString FormatLibrary::convertToFString(const std::string& oriString)
{
	return FString(oriString.c_str());
}

std::string FormatLibrary::convertToStdString(const FString& oriString)
{
	return { TCHAR_TO_UTF8(*oriString) };
}
