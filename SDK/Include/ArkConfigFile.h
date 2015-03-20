//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConfigFile.h
//--------------------------------------------------------------------------------
#ifndef __ArkConfigFile_h__
#define __ArkConfigFile_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFileSystem.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkLog;
	
	struct ArkConfig
	{
		float BuildNumber;
		int DesiredResolutionX;
		int DesiredResolutionY;

		ArkConfig(float bn,int x,int y)
		{
			BuildNumber = bn;
			DesiredResolutionX = x;
			DesiredResolutionY = y;
		}
		ArkConfig() { }
	};
	class ArkConfigFile
	{
	public:
		ArkConfigFile();
		~ArkConfigFile();

		static ArkConfigFile& Get();


	public:
		bool Open();
		bool Close();


		bool Read();

		std::wstring CreateDefaultConfigFile();

		ArkConfig GetConfig();
	private:
		ArkFileSystem	m_cArkFs;
		const char*		m_cData;

		std::wstring	m_cLocation;
		std::wstring	m_sFullConfigPath;

		std::ifstream  m_sFileStream;

		static ArkConfigFile*	gArkConfig;
		ArkConfig				gConfig;
	};
};
//--------------------------------------------------------------------------------
#endif