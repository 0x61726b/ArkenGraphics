//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkVertexShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkVertexShader11::ArkVertexShader11( ID3D11VertexShader* pShader )
{
	m_pVertexShader = pShader;
}
//--------------------------------------------------------------------------------
ArkVertexShader11::~ArkVertexShader11()
{
	COM_Release( m_pVertexShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkVertexShader11::GetType()
{
	return ShaderType::VERTEX_SHADER;
}
//--------------------------------------------------------------------------------