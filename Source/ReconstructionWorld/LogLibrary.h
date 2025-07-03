// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include<iostream>
#include<fstream>
#include<string>

/**
 * 
 */
class RECONSTRUCTIONWORLD_API LogLibrary
{
public:

	std::fstream fileHandle;

	//获取log library的实体
	static LogLibrary* getInstance();

	// 获取它的输出流
	std::ostream& output() {
		return fileHandle;
	}

	LogLibrary();
	~LogLibrary();
};
