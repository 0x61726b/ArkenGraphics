//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11DepthStencilStateConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11DepthStencilStateConfig_h__
#define __Dx11DepthStencilStateConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11DepthStencilStateConfig : public D3D11_DEPTH_STENCIL_DESC
	{
	public:
		Dx11DepthStencilStateConfig();
		~Dx11DepthStencilStateConfig();

		void SetDefaults();

	protected:

		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif