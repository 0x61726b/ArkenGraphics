//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConfigFile.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkConfigFile.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
ArkConfigFile* ArkConfigFile::gArkConfig = 0;
//--------------------------------------------------------------------------------
#define DEFBUILDNUMBER 0.1
#define BUILDNUMBER 0.1
#define DEFRESX 1280
#define DEFRESY 720
//--------------------------------------------------------------------------------
ArkConfigFile::ArkConfigFile()
{
	gArkConfig = this;
	m_cLocation = m_cArkFs.GetWorkingDirectory();
	m_sFullConfigPath = m_cLocation + L"\\Config\\";
}
//--------------------------------------------------------------------------------
ArkConfigFile::~ArkConfigFile()
{
}
//--------------------------------------------------------------------------------
ArkConfigFile& ArkConfigFile::Get()
{
	return *gArkConfig;
}
//--------------------------------------------------------------------------------
bool ArkConfigFile::Open()
{
	//Try to open 
	std::wstring configFile = L"Config.ini";
	if(m_cArkFs.CheckIfDirectoryExists(m_sFullConfigPath))
	{
		if(m_cArkFs.CheckIfFileExists(m_sFullConfigPath + configFile))
		{
			std::ifstream ss;
			ss.open(m_sFullConfigPath + configFile);

			std::string data;
			const char* d;
			while(!ss.eof())
			{
				std::string x;
				std::getline(ss,x);
				x.append("\n");
				data.append(x);
			}
			//Parse

			std::string delimiter = "=";
			std::string newLine = "\n";
			std::string token;
			auto start = 0U;
			auto end = data.find(newLine);
			ArkConfig ac;
			while( ((data.find(newLine)) != std::string::npos ) && end <= data.length() )
			{/*
				token = data.substr(start,end-start);
				start = end + delimiter.length();
				end = data.find(delimiter,start);*/

				token = data.substr(start,end-start);

				//Find config and value
				std::string newToken;
				std::string value;
				size_t innerStart = 0;
				size_t innerEnd = token.find(delimiter);
				if( token.find(delimiter) != std::string::npos )
				{
					newToken = token.substr(innerStart,innerEnd-innerStart);
					value = token.substr(innerEnd-innerStart+1,token.length());

					
					
					if( newToken == "Build" )
					{
						ac.BuildNumber.assign(value.begin(),value.end());
					}
					if( newToken == "ResX" )
					{
						ac.DesiredResolutionX = std::stoi(value);
					}
					if( newToken == "ResY" )
					{
						ac.DesiredResolutionY = std::stoi(value);
					}
					
					
				}

				start = end + newLine.length();
				end = data.find(newLine,start);
			}
			gConfig = ac;
		}
	}
	else
	{
		if(m_cArkFs.ArkCreateDirectory(m_sFullConfigPath))
		{
			CreateDefaultConfigFile();

		}
	}
	return true;
}
//--------------------------------------------------------------------------------
ArkConfig ArkConfigFile::GetConfig()
{
	return gConfig;
}
//--------------------------------------------------------------------------------
std::wstring ArkConfigFile::CreateDefaultConfigFile()
{
	std::wofstream wofs;
	wofs.open(m_sFullConfigPath + L"Config.ini");
	std::wstring data;
	const char* newLine = "\n";
	const char* eof = "--End--";
	wofs << L"Build=" << BUILDNUMBER<< newLine;
	wofs << L"ResX=" << DEFRESX << newLine;
	wofs << L"ResY=" << DEFRESY << newLine;
	
	wofs.flush();
	wofs.close();

	return data;
}
//--------------------------------------------------------------------------------
bool ArkConfigFile::Close()
{
	return true;
}
//--------------------------------------------------------------------------------
bool ArkConfigFile::Read()
{
	return true;
}
//--------------------------------------------------------------------------------