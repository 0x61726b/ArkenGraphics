//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkFileSystem.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFileSystem.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkFileSystem::ArkFileSystem()
{
}
//--------------------------------------------------------------------------------
ArkFileSystem::~ArkFileSystem()
{
}
//--------------------------------------------------------------------------------
bool ArkFileSystem::CheckIfFileExists(const std::wstring& file)
{
	DWORD attr = GetFileAttributes(file.c_str());
	return attr != INVALID_FILE_ATTRIBUTES;
}
//--------------------------------------------------------------------------------
bool ArkFileSystem::CheckIfDirectoryExists(const std::wstring& file)
{
	DWORD attr = GetFileAttributes(file.c_str());
	return attr != INVALID_FILE_ATTRIBUTES && ( attr & FILE_ATTRIBUTE_DIRECTORY ) ;
}
//--------------------------------------------------------------------------------
bool ArkFileSystem::ArkCreateDirectory(const std::wstring& dir)
{
	return CreateDirectory(dir.c_str(),NULL);
}
//--------------------------------------------------------------------------------
const std::wstring ArkFileSystem::GetWorkingDirectory()
{
	char* full = new char[_MAX_PATH];
	if (_fullpath(full,"..\\..",_MAX_PATH ) == NULL )
	{
		ArkLog::Get( LogType::FileSystem ).Output(L"Can't get working directory.");
	}
	std::wstring dir(full,full+strlen(full));
	
	return dir;
}
//--------------------------------------------------------------------------------
const std::wstring ArkFileSystem::GetShadersDirectory()
{
	return GetWorkingDirectory() + L"\\Resources\\Shaders\\";
}