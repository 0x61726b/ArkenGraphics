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
DxRasterizerState::DxRasterizerState()
{
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
void DxRasterizerState::Clear()
{
	m_vViewports.clear();
}
//--------------------------------------------------------------------------------
void DxRasterizerState::AddViewport( int viewport )
{
	m_vViewports.push_back(viewport);
}
//--------------------------------------------------------------------------------
void DxRasterizerState::SetViewportCount(unsigned int count)
{
	assert( count < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE );

	m_iViewportCount = count;
}
//--------------------------------------------------------------------------------
int DxRasterizerState::GetViewportCount()
{
	return m_iViewportCount;
}