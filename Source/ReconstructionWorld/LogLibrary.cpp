// Fill out your copyright notice in the Description page of Project Settings.


#include "LogLibrary.h"
#include<iostream>

static LogLibrary* libraryInstance;
uint32_t initFlag;

LogLibrary* LogLibrary::getInstance()
{
	if (initFlag != 22) {
		initFlag = 22;
		libraryInstance = new LogLibrary();
	}
	return libraryInstance;
}

LogLibrary::LogLibrary()
{
	this->fileHandle.open("E:/temp/UE_Log.txt",std::ios::out);
}

LogLibrary::~LogLibrary()
{
}
