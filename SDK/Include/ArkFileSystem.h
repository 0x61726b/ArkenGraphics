//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkFileSystem.h
//--------------------------------------------------------------------------------
#ifndef __ArkFileSystem_h__
#define __ArkFileSystem_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkLog;

	class ArkFileSystem
	{
	public:
		ArkFileSystem();
		~ArkFileSystem();

		bool CheckIfFileExists(const std::wstring& file);
		bool CheckIfDirectoryExists(const std::wstring& dir);

		bool ArkCreateDirectory(const std::wstring& dir);

		const std::wstring GetWorkingDirectory();

		
	};
};
//--------------------------------------------------------------------------------
#endif