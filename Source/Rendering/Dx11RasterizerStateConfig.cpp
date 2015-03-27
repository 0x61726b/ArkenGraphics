//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RasterizerStateConfig.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11RasterizerStateConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
Dx11RasterizerStateConfig::Dx11RasterizerStateConfig()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11RasterizerStateConfig::~Dx11RasterizerStateConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11RasterizerStateConfig::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	FillMode = D3D11_FILL_SOLID;
	CullMode = D3D11_CULL_BACK;
	FrontCounterClockwise = false;
	DepthBias = 0;
	SlopeScaledDepthBias = 0.0f;
	DepthBiasClamp = 0.0f;
	DepthClipEnable = true;
	ScissorEnable = false;
	MultisampleEnable = false;
	AntialiasedLineEnable = false;
}
//--------------------------------------------------------------------------------
