//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderStage11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShaderStage11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
ArkShaderStage11::ArkShaderStage11()
{
}
//--------------------------------------------------------------------------------
ArkShaderStage11::~ArkShaderStage11()
{
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::ClearState()
{
	State.ClearState();
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::ApplyState( ID3D11DeviceContext* pContext )
{
	BindShaderProgram( pContext );
	BindConstantBuffers( pContext );
}
