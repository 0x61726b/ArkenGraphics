//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShader11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkHullShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkHullShader11::ArkHullShader11( ID3D11HullShader* pShader )
{
	m_pHullShader = pShader;
}
//--------------------------------------------------------------------------------
ArkHullShader11::~ArkHullShader11()
{
	COM_Release( m_pHullShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkHullShader11::GetType()
{
	return( HULL_SHADER );
}
//--------------------------------------------------------------------------------