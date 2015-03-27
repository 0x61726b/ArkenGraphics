//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11BlendStateConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11BlendStateConfig_h__
#define __Dx11BlendStateConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11BlendStateConfig : public D3D11_BLEND_DESC 
	{
	public:
		Dx11BlendStateConfig();
		~Dx11BlendStateConfig();

		void SetDefaults();

	protected:
		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif
