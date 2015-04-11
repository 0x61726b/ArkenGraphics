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
	CurrentState.SetPreviousState(&PreviousState);
}
//--------------------------------------------------------------------------------
ArkShaderStage11::~ArkShaderStage11()
{
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::SetFeatureLevel(D3D_FEATURE_LEVEL FeatureLevel)
{
	m_FeatureLevel = FeatureLevel;

	CurrentState.SetFeautureLevel(FeatureLevel);
	PreviousState.SetFeautureLevel(FeatureLevel);
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::ClearCurrentState()
{
	CurrentState.ClearState();
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::ClearPreviousState()
{
	PreviousState.ClearState();
}
//--------------------------------------------------------------------------------
void ArkShaderStage11::ApplyCurrentState(ID3D11DeviceContext* pContext)
{
	if(CurrentState.ShaderProgram.IsUpdateNeeded())
	{
		BindShaderProgram(pContext);
	}
	
	if(CurrentState.ConstantBuffers.IsUpdateNeeded())
	{
		BindConstantBuffers(pContext,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT-1);
	}

	if( CurrentState.SamplerStates.IsUpdateNeeded() )
		BindSamplerStates( pContext,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT-1 );

	if( CurrentState.ShaderResourceViews.IsUpdateNeeded() )
		BindShaderResourceViews( pContext,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT-1);

	if ( CurrentState.UnorderedAccessViews.IsUpdateNeeded() 
		|| CurrentState.UAVInitialCounts.IsUpdateNeeded() ) {
		if ( m_FeatureLevel != D3D_FEATURE_LEVEL_11_0 )
			BindUnorderedAccessViews( pContext, 1 );
		else
			BindUnorderedAccessViews( pContext, D3D11_PS_CS_UAV_REGISTER_COUNT-1 );
	}

	CurrentState.ResetUpdate();
	PreviousState = CurrentState;
}
