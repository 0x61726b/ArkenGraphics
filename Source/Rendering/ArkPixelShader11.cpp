//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkPixelShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkPixelShader11::ArkPixelShader11( ID3D11PixelShader* pShader )
{
	m_pPixelShader = pShader;
}
//--------------------------------------------------------------------------------
ArkPixelShader11::~ArkPixelShader11()
{
	COM_Release( m_pPixelShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkPixelShader11::GetType()
{
	return ShaderType::PIXEL_SHADER;
}
//--------------------------------------------------------------------------------