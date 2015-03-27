//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RasterizerStateConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11RasterizerStateConfig_h__
#define __Dx11RasterizerStateConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11RasterizerStateConfig : public D3D11_RASTERIZER_DESC 
	{
	public:
		Dx11RasterizerStateConfig();
		~Dx11RasterizerStateConfig();

		void SetDefaults();

	protected:
		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif