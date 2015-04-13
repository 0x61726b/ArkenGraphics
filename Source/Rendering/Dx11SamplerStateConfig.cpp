//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11SamplerStateConfig.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11SamplerStateConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11SamplerStateConfig::Dx11SamplerStateConfig()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11SamplerStateConfig::~Dx11SamplerStateConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11SamplerStateConfig::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	MipLODBias = 0.0f;
	MaxAnisotropy = 1;
	ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	BorderColor[0] = 0.0f; BorderColor[1] = 0.0f; BorderColor[2] = 0.0f; BorderColor[3] = 0.0f;
	MinLOD = 0;
	MaxLOD = D3D11_FLOAT32_MAX;
}
//--------------------------------------------------------------------------------
