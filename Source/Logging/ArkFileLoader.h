//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkFileLoader.h
//--------------------------------------------------------------------------------
#ifndef __ArkFileLoader_h__
#define __ArkFileLoader_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFileLoader
	{
	public:
		ArkFileLoader();
		~ArkFileLoader();

		bool Open( const std::wstring& filename );
		bool Close( );

		char* GetDataPtr();
		unsigned int GetDataSize();

	protected:
		char*			m_pData;
		unsigned int	m_uiSize;
	};
};

//--------------------------------------------------------------------------------
#endif