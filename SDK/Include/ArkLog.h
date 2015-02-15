//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Logging.h
//--------------------------------------------------------------------------------
#ifndef __Log_h__
#define __Log_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFileSystem.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	enum LogType
	{
		Base,
		System,
		Renderer,
		WindowsEvents,
		FileSystem
	};
	class ArkLog
	{
	public:
		ArkLog(LogType lt);
		virtual ~ArkLog();
		std::wofstream AppLog;
	public:
		static ArkLog& Get( LogType lt );

		bool Open();
		bool Close();

		bool Write( const wchar_t *Text );
		bool Write( std::wstring& Text );

		bool Output( std::wstring& Text );
		bool Output( const wchar_t* Text );
		virtual LogType GetType();

		const std::wstring GetCurrentDateTime();

	protected:
		std::wstring	m_sOutput;
		static LogType m_eType;

	private:
		static ArkLog*		m_sLog;
		ArkFileSystem   m_cArkFs;
		
	};
};
//--------------------------------------------------------------------------------
#endif