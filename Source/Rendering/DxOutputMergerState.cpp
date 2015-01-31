//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//DxOutputMergerState.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "DxOutputMergerState.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
DxOutputMergerState::DxOutputMergerState()
{
}
//--------------------------------------------------------------------------------
DxOutputMergerState::~DxOutputMergerState()
{
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::SetFeatureLevel( D3D_FEATURE_LEVEL FeatureLevel )
{
	m_eFeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
int DxOutputMergerState::GetRenderTargetCount() const
{
	return RenderTargetViews.size();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::ClearState()
{
	
}

