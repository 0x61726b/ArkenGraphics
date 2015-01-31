//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "PipelineManager.h"
#include "Dx11Resource.h"
#include "Dx11RenderTargetView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
PipelineManager::PipelineManager()
{
}
//--------------------------------------------------------------------------------
PipelineManager::~PipelineManager()
{
	if( m_pContext ) m_pContext->ClearState();
	if( m_pContext ) m_pContext->Flush();
}
//--------------------------------------------------------------------------------
void PipelineManager::SetDeviceContext( DeviceContextComPtr Context,D3D_FEATURE_LEVEL FeatureLevel )
{
	m_pContext = Context;
	m_FeatureLevel = FeatureLevel;

	OutputMergerStage.SetFeatureLevel( FeatureLevel );
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyRenderTargets()
{
	OutputMergerStage.ApplyRenderTargets( m_pContext.Get() );
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearRenderTargets()
{
	OutputMergerStage.ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineState()
{
	OutputMergerStage.ClearState();

	m_pContext->ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearBuffers(float color[],float depth)
{
	ID3D11RenderTargetView* pRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { NULL };
	UINT viewCount = OutputMergerStage.GetCurrentState().GetRenderTargetCount();

	for( UINT i = 0; i < viewCount; ++i )
	{
		int rtv = OutputMergerStage.GetCurrentState().RenderTargetViews[i];
		Dx11RenderTargetView& rtView = D3D11Renderer::Get()->GetRenderTargetViewByIndex(rtv);
		pRenderTargetViews[i] = rtView.m_pRenderTargetView.Get();

		if( pRenderTargetViews[i] != nullptr )
		{
			m_pContext->ClearRenderTargetView(pRenderTargetViews[i],color);
		}
	}
}
//--------------------------------------------------------------------------------