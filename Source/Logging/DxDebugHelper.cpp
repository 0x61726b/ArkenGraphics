//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxDebugHelper.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxDebugHelper.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxDebugHelper::DxDebugHelper()
{
}
//--------------------------------------------------------------------------------
DxDebugHelper& DxDebugHelper::Get()
{
	static DxDebugHelper dxdebug;
	return dxdebug;
}
//--------------------------------------------------------------------------------
template<UINT T>
void DxDebugHelper::SetDebugObjectName(_In_ ID3D11DeviceChild* resource,
	_In_z_ const char(&name)[T])
{
#if defined(_DEBUG) || defined(PROFILE)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName,TNameLength - 1,name);
#endif
}