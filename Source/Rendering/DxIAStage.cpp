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
}
//--------------------------------------------------------------------------------
void DxIAStage::ClearState()
{
	CurrentState.Clear();
}
//--------------------------------------------------------------------------------
void DxIAStage::ApplyState(ID3D11DeviceContext* pContext)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	InputLayoutComPtr pLayout = pRenderer->GetInputLayout(CurrentState.GetInputLayout());

	if(pLayout)
	{
		pContext->IASetInputLayout(pLayout.Get());
	}
	pContext->IASetPrimitiveTopology(CurrentState.GetPrimitiveTopology());

	ID3D11Buffer* Buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] ={NULL};

	std::vector<int> VertexBuffers = CurrentState.GetVertexBuffers();
	for(unsigned int i = 0; i < CurrentState.GetAvailableSlotCount(); i++)
	{
		int index = VertexBuffers[i];

		ArkVertexBuffer11* pBuffer = pRenderer->GetVertexBufferByIndex(index);

		if(pBuffer) {
			Buffers[i] = static_cast<ID3D11Buffer*>(pBuffer->GetResource());
		}
		else {
			Buffers[i] = 0;
		}
	}

	UINT startSlot = 0;
	std::vector<unsigned int> strides = CurrentState.GetStrides();
	std::vector<unsigned int> offsets = CurrentState.GetOffsets();

	pContext->IASetVertexBuffers(
		startSlot,
		CurrentState.GetVertexBuffers().size(),
		Buffers,
		&strides[0],
		&offsets[0]
		);

	int index = CurrentState.GetIndexBuffer();

	ArkIndexBuffer11* pBuffer = pRenderer->GetIndexBufferByIndex(index);

	if(pBuffer) {
		ID3D11Buffer* pIndexBuffer = reinterpret_cast<ID3D11Buffer*>(pBuffer->GetResource());
		pContext->IASetIndexBuffer(pIndexBuffer,CurrentState.GetIndexBufferFormat(),0);
	}
	else {
		pContext->IASetIndexBuffer(0,CurrentState.GetIndexBufferFormat(),0);
	}

}
//--------------------------------------------------------------------------------
