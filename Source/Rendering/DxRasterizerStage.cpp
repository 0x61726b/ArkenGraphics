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
}
//--------------------------------------------------------------------------------
DxRasterizerStage::~DxRasterizerStage()
{
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::ClearState()
{
	State.Clear();
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;

	State.SetFeatureLevel( m_FeatureLevel );
}
//--------------------------------------------------------------------------------
void DxRasterizerStage::ApplyState( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	D3D11_VIEWPORT Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	for( int i=0; i < State.GetViewportCount(); ++i )
	{
		const Dx11ViewPort& Viewport = pRenderer->GetViewPort( State.m_vViewports[i] );
		Viewports[i] = Viewport.m_ViewPort;


	}
	pContext->RSSetViewports( State.GetViewportCount(),Viewports );

}
const DxRasterizerState& DxRasterizerStage::GetCurrentState() const
{
	return State;
}
//--------------------------------------------------------------------------------