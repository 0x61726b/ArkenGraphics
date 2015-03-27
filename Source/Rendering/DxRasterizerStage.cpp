//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxRasterizerStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderer11.h"
#include "DxRasterizerStage.h"
#include "Dx11ViewPort.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxRasterizerStage::DxRasterizerStage()
{
	CurrentState.SetPreviousState( &PreviousState );
}
//--------------------------------------------------------------------------------
DxRasterizerStage::~DxRasterizerStage()
{
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::ClearCurrentState()
{
	CurrentState.ClearState();
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::ClearPreviousState()
{
	PreviousState.ClearState();
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;

	CurrentState.SetFeatureLevel( m_FeatureLevel );
	PreviousState.SetFeatureLevel( m_FeatureLevel );
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::ApplyCurrentState( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	if( CurrentState.RasterizerState.IsUpdateNeeded() )
	{
		RasterizerStateComPtr pArkRasterizerState = pRenderer->GetRasterizerState( CurrentState.RasterizerState.GetState() );

		if( pArkRasterizerState != nullptr )
		{
			ID3D11RasterizerState* pRasterizerState = pArkRasterizerState.Get();
			pContext->RSSetState( pRasterizerState );
		}
	}

	int viewports = 0;

	if( CurrentState.ViewPorts.IsUpdateNeeded() || CurrentState.ViewPortCount.IsUpdateNeeded() )
	{
		D3D11_VIEWPORT aViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

		for( int i=0; i < CurrentState.ViewPortCount.GetState(); ++i )
		{
			const Dx11ViewPort& ViewPort = pRenderer->GetViewPort( CurrentState.ViewPorts.GetState(i) );

			aViewports[i] = ViewPort.m_ViewPort;
		}
		pContext->RSSetViewports(CurrentState.ViewPortCount.GetState(),aViewports);
	}
	if ( CurrentState.ScissorRectCount.IsUpdateNeeded() || CurrentState.ScissorRects.IsUpdateNeeded() ) {
		
		D3D11_RECT aRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		
		for ( int i = 0; i < CurrentState.ScissorRectCount.GetState(); i++ ) {
			aRects[i] = CurrentState.ScissorRects.GetState( i );
		}

		pContext->RSSetScissorRects( CurrentState.ScissorRectCount.GetState(), aRects );
	}
	CurrentState.ResetUpdateFlags();
	PreviousState = CurrentState;

}
const DxRasterizerState& DxRasterizerStage::GetCurrentState() const
{
	return CurrentState;
}
//--------------------------------------------------------------------------------