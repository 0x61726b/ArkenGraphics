//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxOutputMergerState.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxOutputMergerStage.h"
#include "Dx11RenderTargetView.h"

//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxOutputMergerStage::DxOutputMergerStage()
{
}
//--------------------------------------------------------------------------------
DxOutputMergerStage::~DxOutputMergerStage()
{
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_eFeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ClearState()
{
	State.ClearState();
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ApplyRenderTargets( ID3D11DeviceContext* pContext )
{
	const int rtvCount = State.GetRenderTargetCount();

	if(rtvCount > 0 )
	{
		ArkRenderer11* pRenderer = ArkRenderer11::Get();
		
		
		std::vector< ID3D11RenderTargetView* > rtvs( rtvCount );

		for( int i=0; i < rtvCount; ++i )
		{
			Dx11RenderTargetView& rView = pRenderer->GetRenderTargetViewByIndex( State.GetRenderTargetView( i ) );
			rtvs[i] = rView.m_pRenderTargetView.Get();
			if( rtvs[i] == nullptr )
			{
			}
		}

		pContext->OMSetRenderTargets(rtvCount,&rtvs[0],NULL);
	}
}
//--------------------------------------------------------------------------------
const DxOutputMergerState& DxOutputMergerStage::GetCurrentState() const
{
	return State;
}
//--------------------------------------------------------------------------------



