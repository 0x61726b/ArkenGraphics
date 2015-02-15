//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxDebugHelper.h
//--------------------------------------------------------------------------------
#ifndef __DxDebugHelper_h__
#define __DxDebugHelper_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class DxDebugHelper
	{
	public:
		DxDebugHelper();

		static DxDebugHelper& Get();

		template<UINT T>
		inline void SetDebugObjectName( _In_ ID3D11DeviceChild* resource,
_In_z_ const char (&name)[T] );
	};
};

//--------------------------------------------------------------------------------
#endif