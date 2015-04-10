//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxIAState.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxIAState.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxIAState::DxIAState() :
	m_uiAvailableSlotCount( 0 ),
	m_FeatureLevel( D3D_FEATURE_LEVEL_9_1 ),
	m_pPreviousState( nullptr ),
	InputLayout(-1),
	PrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED ),
	VertexBuffers( -1 ),
	VertexBufferStrides( 0 ),
	VertexBufferOffsets( 0 ),
	IndexBufferFormat( DXGI_FORMAT_R32_UINT ),
	IndexBuffer( -1 )
{
	ClearState();
}
//--------------------------------------------------------------------------------
DxIAState::~DxIAState()
{
}
//--------------------------------------------------------------------------------
void DxIAState::ClearState()
{

	InputLayout.InitializeState();
	PrimitiveTopology.InitializeState();
	VertexBuffers.InitializeStates();
	VertexBufferOffsets.InitializeStates();
	VertexBufferStrides.InitializeStates();

	IndexBuffer.InitializeState();
	IndexBufferFormat.InitializeState();
}
//--------------------------------------------------------------------------------
void DxIAState::SetPreviousState( DxIAState* pPrev )
{
	m_pPreviousState = pPrev;

	InputLayout.SetSister( &pPrev->InputLayout );
	PrimitiveTopology.SetSister( &pPrev->PrimitiveTopology );
	VertexBuffers.SetSister( &pPrev->VertexBuffers );
	VertexBufferOffsets.SetSister( &pPrev->VertexBufferOffsets );
	VertexBufferStrides.SetSister( &pPrev->VertexBufferStrides );
	IndexBuffer.SetSister( &pPrev->IndexBuffer );
	IndexBufferFormat.SetSister( &pPrev->IndexBufferFormat );
}
//--------------------------------------------------------------------------------
void DxIAState::ResetUpdate()
{
	InputLayout.ResetTracking();
	PrimitiveTopology.ResetTracking();
	VertexBuffers.ResetTracking();
	VertexBufferOffsets.ResetTracking();
	VertexBufferStrides.ResetTracking();
	IndexBuffer.ResetTracking();
	IndexBufferFormat.ResetTracking();
}
//--------------------------------------------------------------------------------
void DxIAState::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;

	if ( m_FeatureLevel == D3D_FEATURE_LEVEL_11_0 ) {
		m_uiAvailableSlotCount = D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
	} else {
		m_uiAvailableSlotCount = D3D10_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
	}
}
//--------------------------------------------------------------------------------
unsigned int DxIAState::GetAvailableSlotCount()
{
	return m_uiAvailableSlotCount;
}
//--------------------------------------------------------------------------------