//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryShader11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkGeometryShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkGeometryShader11::ArkGeometryShader11( ID3D11GeometryShader* pShader )
{
	m_pGeometryShader = pShader;
}
//--------------------------------------------------------------------------------
ArkGeometryShader11::~ArkGeometryShader11()
{
	COM_Release( m_pGeometryShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkGeometryShader11::GetType()
{
	return( GEOMETRY_SHADER );
}
//--------------------------------------------------------------------------------