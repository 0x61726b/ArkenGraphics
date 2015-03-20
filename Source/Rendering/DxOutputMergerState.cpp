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
void DxOutputMergerState::SetFeatureLevel(D3D_FEATURE_LEVEL FeatureLevel)
{
	m_eFeatureLevel = FeatureLevel;
}
//--------------------------------------------------------------------------------
int DxOutputMergerState::GetRenderTargetCount() const
{
	return m_vRenderTargetViews.size();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::ClearState()
{
	m_vRenderTargetViews.clear();
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::AddRenderTargetView(int ID)
{
	for(int i=0; i < m_vRenderTargetViews.size(); ++i)
	{
		if(m_vRenderTargetViews[i] == ID)
			return;
	}
	m_vRenderTargetViews.push_back(ID);
}
//--------------------------------------------------------------------------------
int DxOutputMergerState::GetRenderTargetView(int ID)
{
	return m_vRenderTargetViews[ID];
}
//--------------------------------------------------------------------------------
bool DxOutputMergerState::CheckIfRTVExists(int ID)
{
	//for(int i=0; i < m_vRenderTargetViews.size(); ++i)
	//{
	//	if(m_vRenderTargetViews[] == ID)
	//		return true;
	//}
	return false;
}
//--------------------------------------------------------------------------------
void DxOutputMergerState::AddDepthTarget(int ID)
{
	m_iDepthTarget = ID;
}
//--------------------------------------------------------------------------------
int DxOutputMergerState::GetDepthTarget()
{
	return m_iDepthTarget;
}
