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
	DepthTarget(0),
	BlendState( -1 ),
	DepthStencilState( -1 ),
	StencilRef(0)
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
	RenderTargetViews.InitializeStates();
	DepthTarget.InitializeState();

	BlendState.InitializeState();
	DepthStencilState.InitializeState();
	StencilRef.InitializeState();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::ResetUpdate()
{
	RenderTargetViews.ResetTracking();
	DepthTarget.ResetTracking();

	BlendState.InitializeState();
	DepthStencilState.InitializeState();
	StencilRef.InitializeState();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::SetPreviosState( DxOutputMergerState* pPrev )
{
	m_pPrevState = pPrev;

	RenderTargetViews.SetSister( &pPrev->RenderTargetViews );
	DepthTarget.SetSister( &pPrev->DepthTarget );
	BlendState.SetSister( &pPrev->BlendState );
	DepthStencilState.SetSister( &pPrev->DepthStencilState );
	StencilRef.SetSister( &pPrev->StencilRef );
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