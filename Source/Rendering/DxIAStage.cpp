//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxIAStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxIAStage.h"
#include "ArkRenderer11.h"
#include "DxIAStage.h"
#include "ArkVertexBuffer11.h"
#include "ArkIndexBuffer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxIAStage::DxIAStage()
{
	CurrentState.SetPreviousState(&PreviousState);
}
//--------------------------------------------------------------------------------
DxIAStage::~DxIAStage()
{
}
//--------------------------------------------------------------------------------
void DxIAStage::SetFeatureLevel(D3D_FEATURE_LEVEL feature)
{
	FeatureLevel = feature;

	CurrentState.SetFeatureLevel(feature);
	PreviousState.SetFeatureLevel(feature);
}
//--------------------------------------------------------------------------------
void DxIAStage::ClearCurrentState()
{
	CurrentState.ClearState();
}
//--------------------------------------------------------------------------------
void DxIAStage::ClearPreviousState()
{
	PreviousState.ClearState();
}
//--------------------------------------------------------------------------------
void DxIAStage::ApplyCurrentState(ID3D11DeviceContext* pContext)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	if(CurrentState.InputLayout.IsUpdateNeeded())
	{
		InputLayoutComPtr pLayout = pRenderer->GetInputLayout(CurrentState.InputLayout.GetState());

		if(pLayout)
		{
			pContext->IASetInputLayout(pLayout.Get());
		}
	}

	if(CurrentState.PrimitiveTopology.IsUpdateNeeded())
		pContext->IASetPrimitiveTopology(CurrentState.PrimitiveTopology.GetState());

	if(CurrentState.VertexBuffers.IsUpdateNeeded() || CurrentState.VertexBufferOffsets.IsUpdateNeeded() || CurrentState.VertexBufferStrides.IsUpdateNeeded())
	{

		ID3D11Buffer* Buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] ={NULL};

		for(unsigned int i = 0; i < CurrentState.GetAvailableSlotCount(); i++)
		{
			int index = CurrentState.VertexBuffers.GetState(i);

			std::shared_ptr<ArkVertexBuffer11> pBuffer = pRenderer->GetVertexBufferByIndex(index);

			if(pBuffer) {
				Buffers[i] = static_cast<ID3D11Buffer*>(pBuffer->GetResource());
			}
			else {
				Buffers[i] = 0;
			}
		}

		UINT startSlot = min(CurrentState.VertexBuffers.GetStartSlot(),
			min(CurrentState.VertexBufferOffsets.GetStartSlot(),
			CurrentState.VertexBufferStrides.GetStartSlot()));

		UINT endSlot = max(CurrentState.VertexBuffers.GetEndSlot(),
			max(CurrentState.VertexBufferOffsets.GetEndSlot(),
			CurrentState.VertexBufferStrides.GetEndSlot()));

		pContext->IASetVertexBuffers(
			startSlot,
			endSlot-startSlot+1,
			&Buffers[startSlot],
			CurrentState.VertexBufferStrides.GetSlotLocation(startSlot),
			CurrentState.VertexBufferOffsets.GetSlotLocation(startSlot)
			);
	}

	if(CurrentState.IndexBuffer.IsUpdateNeeded())
	{
		int index = CurrentState.IndexBuffer.GetState();

		std::shared_ptr<ArkIndexBuffer11> pBuffer = pRenderer->GetIndexBufferByIndex(index);

		if(pBuffer) {
			ID3D11Buffer* pIndexBuffer = reinterpret_cast<ID3D11Buffer*>(pBuffer->GetResource());
			pContext->IASetIndexBuffer(pIndexBuffer,CurrentState.IndexBufferFormat.GetState(),0);
		}
		else {
			pContext->IASetIndexBuffer(0,CurrentState.IndexBufferFormat.GetState(),0);
		}
	}
	CurrentState.ResetUpdate();
	PreviousState = CurrentState;
}
//--------------------------------------------------------------------------------
