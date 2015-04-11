//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShader11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkComputeShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkComputeShader11::ArkComputeShader11( ID3D11ComputeShader* pShader )
{
	m_pComputeShader = pShader;
}
//--------------------------------------------------------------------------------
ArkComputeShader11::~ArkComputeShader11()
{
	COM_Release( m_pComputeShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkComputeShader11::GetType()
{
	return( COMPUTE_SHADER );
}
//--------------------------------------------------------------------------------