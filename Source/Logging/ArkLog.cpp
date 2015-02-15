//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Log.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
ArkLog* ArkLog::m_sLog = 0;
LogType ArkLog::m_eType = LogType::Base;
//--------------------------------------------------------------------------------
ArkLog::ArkLog(LogType lt)
{
	m_sLog = this;
	m_sOutput = L"";

	m_eType = lt;
}
//--------------------------------------------------------------------------------
ArkLog::~ArkLog()
{
	delete m_sLog;
}
//--------------------------------------------------------------------------------
ArkLog& ArkLog::Get(LogType lt)
{
	m_eType = lt;
	return *m_sLog;
}
//--------------------------------------------------------------------------------
bool ArkLog::Write(std::wstring& Text)
{
	ArkLog::Write(Text.c_str());
	AppLog.flush();
	return true;
}
//--------------------------------------------------------------------------------
bool ArkLog::Write(const wchar_t *Text)
{
	AppLog << Text << "\n";
#if _DEBUG
	::OutputDebugStringW(Text);
	::OutputDebugStringW(L"\n");
#endif
	return true;
}
//--------------------------------------------------------------------------------
bool ArkLog::Open()
{
	std::wstring dirName = L"C:\\Users\\alperen\\Desktop\\ArkenGraphics\\Logs";
	if( !m_cArkFs.CheckIfDirectoryExists(dirName) )
	{
		m_cArkFs.ArkCreateDirectory(dirName);
	}

	std::wstring seperator = L"\\";
	std::wstring extension = L".arklog";
	std::wstring filename = dirName + seperator + L"Log " + GetCurrentDateTime() + extension;
	
	AppLog.open(filename);

	Output(L"Log file opened.");

	return(true);
}
//--------------------------------------------------------------------------------
bool ArkLog::Close()
{
	Output(L"Log file closed");
	AppLog.close();
	return true;
}
//--------------------------------------------------------------------------------
bool ArkLog::Output(std::wstring& Text)
{
	Output(Text.c_str());
	AppLog.flush();
	return true;
}
//--------------------------------------------------------------------------------
bool ArkLog::Output(const wchar_t* Text)
{
	std::wstring start;
	LogType t = m_eType;

	std::wstring currentDate = GetCurrentDateTime();

	m_sOutput.append(L"[");
	m_sOutput.append(currentDate);
	m_sOutput.append(L"] ");

	switch(t)
	{
	case LogType::Base:
		start = L"\t [Base] -> \t";
		break;
	case LogType::System:
		start = L"\t [System] -> \t";
		break;
	case LogType::Renderer:
		start = L"\t [Renderer] ->\t";
		break;
	case LogType::WindowsEvents:
		start = L"\t [WindowsEvents] -> \t";
		break;
	}
	m_sOutput.append(start);


	const wchar_t* cStr = Text;
	m_sOutput.append(cStr);

	AppLog << m_sOutput.c_str() << "\n";
#if _DEBUG
	::OutputDebugStringW(m_sOutput.c_str());
	::OutputDebugStringW(L"\n");
#endif
	m_sOutput.clear();
	return true;
}
//--------------------------------------------------------------------------------
LogType ArkLog::GetType()
{
	return LogType::Base;
}
//--------------------------------------------------------------------------------
const std::wstring ArkLog::GetCurrentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct,&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf,sizeof(buf),"%Y-%m-%d.%H-%M-%S",&tstruct);
	size_t size = strlen(buf);

	int reqLen = MultiByteToWideChar(CP_UTF8 ,0, buf,size,0,0 );

	wchar_t* d = 0;
	std::wstring w;
	w.resize(strlen(buf));
	

	MultiByteToWideChar(CP_UTF8 ,0, buf,size,&w[0],reqLen );

	return w;

}
//--------------------------------------------------------------------------------