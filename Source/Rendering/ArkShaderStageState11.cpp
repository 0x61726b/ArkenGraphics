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
	SamplerStates(nullptr),
	ShaderResourceViews(nullptr),
	ConstantBuffers( nullptr ),
	UnorderedAccessViews( nullptr ),
	UAVInitialCounts( -1 )
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
	ShaderResourceViews.SetSister( &pPrev->ShaderResourceViews );
	SamplerStates.SetSister( &pPrev->SamplerStates );
	UnorderedAccessViews.SetSister( &pPrev->UnorderedAccessViews );
	UAVInitialCounts.SetSister( &pPrev->UAVInitialCounts );
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::ClearState()
{
	ShaderProgram.InitializeState();
	ConstantBuffers.InitializeStates();
	ShaderResourceViews.InitializeStates();
	SamplerStates.InitializeStates();
	UnorderedAccessViews.InitializeStates();
	UAVInitialCounts.InitializeStates();
}
//--------------------------------------------------------------------------------
void ArkShaderStageState11::ResetUpdate()
{
	ShaderProgram.ResetTracking();
	ConstantBuffers.ResetTracking();
	ShaderResourceViews.ResetTracking();
	SamplerStates.ResetTracking();
	UnorderedAccessViews.ResetTracking();
	UAVInitialCounts.ResetTracking();
}
//--------------------------------------------------------------------------------

