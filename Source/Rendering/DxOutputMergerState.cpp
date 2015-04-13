//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxOutputMergerState.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxOutputMergerState.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxOutputMergerState::DxOutputMergerState()
	: RenderTargetViews(0),
	DepthTargetViews(0),
	BlendState( -1 ),
	DepthStencilState( -1 ),
	StencilRef(0),
	UnorderedAccessViews( 0 ),
	UAVInitialCounts( -1 )
{
	ClearState();
}
//--------------------------------------------------------------------------------
DxOutputMergerState::~DxOutputMergerState()
{
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::SetFeatureLevel(D3D_FEATURE_LEVEL FeatureLevel)
{
	m_eFeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::ClearState()
{
	BlendState.InitializeState();
	DepthStencilState.InitializeState();
	StencilRef.InitializeState();
	RenderTargetViews.InitializeStates();
	DepthTargetViews.InitializeState();
	UnorderedAccessViews.InitializeStates();
	UAVInitialCounts.InitializeStates();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::ResetUpdate()
{
	BlendState.ResetTracking();
	DepthStencilState.ResetTracking();
	StencilRef.ResetTracking();
	RenderTargetViews.ResetTracking();
	DepthTargetViews.ResetTracking();
	UnorderedAccessViews.ResetTracking();
	UAVInitialCounts.ResetTracking();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::SetPreviosState( DxOutputMergerState* pPrev )
{
	m_pPrevState = pPrev;

	BlendState.SetSister( &m_pPrevState->BlendState );
	DepthStencilState.SetSister( &m_pPrevState->DepthStencilState );
	StencilRef.SetSister( &m_pPrevState->StencilRef );
	RenderTargetViews.SetSister( &m_pPrevState->RenderTargetViews );
	DepthTargetViews.SetSister( &m_pPrevState->DepthTargetViews );
	UnorderedAccessViews.SetSister( &m_pPrevState->UnorderedAccessViews );
	UAVInitialCounts.SetSister( &m_pPrevState->UAVInitialCounts );
}
//--------------------------------------------------------------------------------
int DxOutputMergerState::GetRenderTargetCount() const 
{
	unsigned int count = 0;

	for ( unsigned int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++ )
	{
		if ( RenderTargetViews.GetState( i ) != -1 )
			count++;
	}

	return( count );
}
//--------------------------------------------------------------------------------