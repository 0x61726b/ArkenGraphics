//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderStageState11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShaderStageState11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkShaderStageState11::ArkShaderStageState11()
{
}
//--------------------------------------------------------------------------------
ArkShaderStageState11::~ArkShaderStageState11()
{
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::SetFeautureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::ClearState()
{
	ShaderProgram = 0;
	ConstantBuffers.clear();
}
//--------------------------------------------------------------------------------

