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
DxIAState::DxIAState()
	:m_eIBFormat( DXGI_FORMAT_R32_UINT ),
	m_ePrimiviteTopology( D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED ),
	m_uiAvailableSlotCount( 0 ),
	m_FeatureLevel( D3D_FEATURE_LEVEL_9_1 )
{
}
//--------------------------------------------------------------------------------
DxIAState::~DxIAState()
{
}
//--------------------------------------------------------------------------------
void DxIAState::Clear()
{
	m_vVertexBuffers.clear();
	m_vInputLayout = 0;
	m_ePrimiviteTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	m_vVertexBufferOffsets.clear();
	m_vVertexBuffers.clear();
	m_iIndexBuffer = -1;
}
//--------------------------------------------------------------------------------
void DxIAState::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_FeatureLevel = FeatureLevel;

	if( FeatureLevel == D3D_FEATURE_LEVEL_11_0 )
	{
		m_uiAvailableSlotCount = D3D11_IA_INDEX_INPUT_RESOURCE_SLOT_COUNT;
	}
}
//--------------------------------------------------------------------------------
unsigned int DxIAState::GetAvailableSlotCount()
{
	return m_uiAvailableSlotCount;
}
//--------------------------------------------------------------------------------
void DxIAState::SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY t )
{
	m_ePrimiviteTopology = t;
}
//--------------------------------------------------------------------------------
void DxIAState::AddVertexBuffer( int vb )
{
	m_vVertexBuffers.push_back( vb );
}
//--------------------------------------------------------------------------------
void DxIAState::AddVertexBufferStride(unsigned int vbs )
{
	m_vVertexBufferStrides.push_back( vbs );
}
//--------------------------------------------------------------------------------
void DxIAState::AddVertexBufferOffsets( unsigned int vbos )
{
	m_vVertexBufferOffsets.push_back( vbos );
}
//--------------------------------------------------------------------------------
void DxIAState::SetIndexBuffer( int ib )
{
	m_iIndexBuffer = ib;
}
//--------------------------------------------------------------------------------
void DxIAState::SetIndexBufferFormat( DXGI_FORMAT format )
{
	m_eIBFormat = format;
}
//--------------------------------------------------------------------------------
void DxIAState::SetInputLayout(int il)
{
	m_vInputLayout = il;
}
//--------------------------------------------------------------------------------
int DxIAState::GetInputLayout()
{
	return m_vInputLayout;
}
//--------------------------------------------------------------------------------
D3D_PRIMITIVE_TOPOLOGY DxIAState::GetPrimitiveTopology()
{
	return m_ePrimiviteTopology;
}
//--------------------------------------------------------------------------------
std::vector<int> DxIAState::GetVertexBuffers()
{
	return m_vVertexBuffers;
}
//--------------------------------------------------------------------------------
std::vector<unsigned int> DxIAState::GetStrides()
{
	return m_vVertexBufferStrides;
}
//--------------------------------------------------------------------------------
std::vector<unsigned int> DxIAState::GetOffsets()
{
	return m_vVertexBufferOffsets;
}
//--------------------------------------------------------------------------------
int DxIAState::GetIndexBuffer()
{
	return m_iIndexBuffer;
}
//--------------------------------------------------------------------------------
DXGI_FORMAT DxIAState::GetIndexBufferFormat()
{
	return m_eIBFormat;
}
