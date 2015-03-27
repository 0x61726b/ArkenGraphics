//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxRasterizerState.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxRasterizerState.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxRasterizerState::DxRasterizerState() :
	RasterizerState(-1),
	ViewPortCount( 0 ),
	ViewPorts( -1 ),
	ScissorRectCount( 0 ),
	ScissorRects( D3D11_RECT() ),
	m_FeatureLevel( D3D_FEATURE_LEVEL_9_1 ),
	m_pPrevState( nullptr )
{
	ClearState();
}
//--------------------------------------------------------------------------------
DxRasterizerState::~DxRasterizerState()
{
}
//--------------------------------------------------------------------------------
void DxRasterizerState::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
void DxRasterizerState::ClearState()
{
	RasterizerState.InitializeState();
	ViewPortCount.InitializeState();
	ViewPorts.InitializeStates();
	ScissorRects.InitializeStates();
	ScissorRectCount.InitializeState();
}
//--------------------------------------------------------------------------------
void DxRasterizerState::SetPreviousState( DxRasterizerState* pPrev )
{
	m_pPrevState = pPrev;

	RasterizerState.SetSister( &pPrev->RasterizerState );
	ViewPortCount.SetSister( &pPrev->ViewPortCount );
	ViewPorts.SetSister( &pPrev->ViewPorts );
	ScissorRects.SetSister( &pPrev->ScissorRects );
	ScissorRectCount.SetSister( &pPrev->ScissorRectCount );
}
//--------------------------------------------------------------------------------
void DxRasterizerState::ResetUpdateFlags()
{
	RasterizerState.ResetTracking();
	ViewPortCount.ResetTracking();
	ViewPorts.ResetTracking();
	ScissorRectCount.ResetTracking();
	ScissorRects.ResetTracking();		
}
