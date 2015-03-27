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
	:ShaderProgram(-1),
	ConstantBuffers( nullptr )
{
	ClearState();
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
void ArkShaderStageState11::SetPreviousState( ArkShaderStageState11* pPrev )
{
	m_pPrevState = pPrev;

	ShaderProgram.SetSister( &pPrev->ShaderProgram );
	ConstantBuffers.SetSister( &pPrev->ConstantBuffers );
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::ClearState()
{
	ShaderProgram.InitializeState();
	ConstantBuffers.InitializeStates();
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::ResetUpdate()
{
	ShaderProgram.ResetTracking();
	ConstantBuffers.ResetTracking();
}
//--------------------------------------------------------------------------------

