//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11SamplerStateConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11SamplerStateConfig_h__
#define __Dx11SamplerStateConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11SamplerStateConfig : public D3D11_SAMPLER_DESC
	{
	public:
		Dx11SamplerStateConfig();
		~Dx11SamplerStateConfig();

		void SetDefaults();

	protected:

		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif // SamplerStateConfigDX11_h
//--------------------------------------------------------------------------------

