//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShader11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkDomainShader11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkDomainShader11::ArkDomainShader11( ID3D11DomainShader* pShader )
{
	m_pDomainShader = pShader;
}
//--------------------------------------------------------------------------------
ArkDomainShader11::~ArkDomainShader11()
{
	COM_Release( m_pDomainShader );
}
//--------------------------------------------------------------------------------
ShaderType ArkDomainShader11::GetType()
{
	return( DOMAIN_SHADER );
}
//--------------------------------------------------------------------------------