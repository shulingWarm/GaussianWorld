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

	//��ȡlog library��ʵ��
	static LogLibrary* getInstance();

	// ��ȡ���������
	std::ostream& output() {
		return fileHandle;
	}

	LogLibrary();
	~LogLibrary();
};
