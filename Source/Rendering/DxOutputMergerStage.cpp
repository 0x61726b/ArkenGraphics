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
#include "Dx11DepthStencilView.h"
#include "Dx11UnorderedAccessView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxOutputMergerStage::DxOutputMergerStage()
{
	CurrentState.SetPreviosState(&PreviousState);
}
//--------------------------------------------------------------------------------
DxOutputMergerStage::~DxOutputMergerStage()
{
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::SetFeatureLevel(D3D_FEATURE_LEVEL FeatureLevel)
{
	m_eFeatureLevel = FeatureLevel;

	CurrentState.SetFeatureLevel(FeatureLevel);
	PreviousState.SetFeatureLevel(FeatureLevel);
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ClearCurrentState()
{
	CurrentState.ClearState();
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ClearPreviousState()
{
	PreviousState.ClearState();
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ApplyCurrentState(ID3D11DeviceContext* pContext)
{
	ApplyRenderTargets(pContext);
	ApplyDepthStencilStatesAndBlendStates(pContext);
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ApplyRenderTargets(ID3D11DeviceContext* pContext)
{
	int rtvCount = 0;
	int uavCount = 0;


	if(CurrentState.RenderTargetViews.IsUpdateNeeded()
		|| CurrentState.DepthTargetViews.IsUpdateNeeded())
	{
		ArkRenderer11* pRenderer = ArkRenderer11::Get();

		ID3D11RenderTargetView* rtvs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

		ID3D11DepthStencilView* dsv = 0;

		ID3D11UnorderedAccessView* uavs[D3D11_PS_CS_UAV_REGISTER_COUNT];

		for(int i=0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		{
			if(CurrentState.RenderTargetViews.GetState(i) == -1)
			{
				for( int j=0; j < 8;j++ )
					rtvs[j] = NULL;
			}
			else
			{

				Dx11RenderTargetView& rView = pRenderer->GetRenderTargetViewByIndex(CurrentState.RenderTargetViews.GetState(i));

				if(rView.m_pRenderTargetView == NULL)
				{
					rtvs[i] = NULL;
				}
				else
				{
					rtvs[i] = rView.m_pRenderTargetView.Get();

					if(rtvs[i] != nullptr)
						rtvCount =  i+1;
				}
			}
		}
		for(int i = 0; i < D3D11_PS_CS_UAV_REGISTER_COUNT; i++)
		{

			Dx11UnorderedAccessView& uav = pRenderer->GetUnorderedAccessViewByIndex(CurrentState.UnorderedAccessViews.GetState(i));
			uavs[i] = uav.m_pUnorderedAccessView.Get();

			if(uavs[i] != nullptr) {
				uavCount = i+1; // Record the number of non-null uavs...
			}
		}

		Dx11DepthStencilView& DSV = pRenderer->GetDepthStencilViewByIndex(CurrentState.DepthTargetViews.GetState());

		dsv = DSV.m_pDepthStencilView.Get();

		pContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,rtvs,dsv);

		for(int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) 
		{
			int index = CurrentState.RenderTargetViews.GetState(i);
			PreviousState.RenderTargetViews.SetState(i,index == -1 ? -1 : index );
		}

		for(int i = 0; i < D3D11_PS_CS_UAV_REGISTER_COUNT; i++) {
			PreviousState.UnorderedAccessViews.SetState(i,CurrentState.UnorderedAccessViews.GetState(i));
			PreviousState.UAVInitialCounts.SetState(i,CurrentState.UAVInitialCounts.GetState(i));
		}

		PreviousState.DepthTargetViews.SetState(CurrentState.DepthTargetViews.GetState());

		CurrentState.RenderTargetViews.ResetTracking();
		CurrentState.UnorderedAccessViews.ResetTracking();
		CurrentState.UAVInitialCounts.ResetTracking();
		CurrentState.DepthTargetViews.ResetTracking();

	}
}
//--------------------------------------------------------------------------------
void DxOutputMergerStage::ApplyDepthStencilStatesAndBlendStates(ID3D11DeviceContext* pContext)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	if(CurrentState.BlendState.IsUpdateNeeded())
	{
		BlendStateComPtr pArkBlendState = pRenderer->GetBlendState(CurrentState.BlendState.GetState());

		if(nullptr != pArkBlendState)
		{
			ID3D11BlendState* pBlendState = pArkBlendState.Get();

			if(pBlendState)
			{
				float afBlendFactors[] ={1.0f,1.0f,1.0f,1.0f};
				pContext->OMSetBlendState(pBlendState,afBlendFactors,0xFFFFFFFF);
			}
			PreviousState.BlendState.SetState(CurrentState.BlendState.GetState());
			CurrentState.BlendState.ResetTracking();
		}
	}

	if(CurrentState.DepthStencilState.IsUpdateNeeded() || CurrentState.StencilRef.IsUpdateNeeded())
	{
		DepthStencilStateComPtr pArkDepthStencilState = pRenderer->GetDepthState(CurrentState.DepthStencilState.GetState());
		if(nullptr != pArkDepthStencilState)
		{
			ID3D11DepthStencilState* pDepthStencilState = pArkDepthStencilState.Get();

			if(pDepthStencilState)
			{
				pContext->OMSetDepthStencilState(pDepthStencilState,CurrentState.StencilRef.GetState());
			}
			PreviousState.DepthStencilState.SetState(CurrentState.DepthStencilState.GetState());
			PreviousState.StencilRef.SetState(CurrentState.StencilRef.GetState());
			CurrentState.DepthStencilState.ResetTracking();
			CurrentState.StencilRef.ResetTracking();
		}
	}
}
//--------------------------------------------------------------------------------
const DxOutputMergerState& DxOutputMergerStage::GetPreviousState() const
{
	return PreviousState;
}
//--------------------------------------------------------------------------------



