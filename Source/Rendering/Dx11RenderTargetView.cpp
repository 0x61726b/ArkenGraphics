//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11RenderTargetView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11RenderTargetView::Dx11RenderTargetView( RenderTargetViewComPtr pView )
{
	m_pRenderTargetView = pView;
}
//--------------------------------------------------------------------------------
Dx11RenderTargetView::~Dx11RenderTargetView()
{
}
//--------------------------------------------------------------------------------
ID3D11RenderTargetView* Dx11RenderTargetView::GetRTV()
{
	return m_pRenderTargetView.Get();
}
//--------------------------------------------------------------------------------

