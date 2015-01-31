//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11DepthStencilView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11DepthStencilView::Dx11DepthStencilView( DepthStencilViewComPtr pView )
{
	m_pDepthStencilView = pView;
}
//--------------------------------------------------------------------------------
Dx11DepthStencilView::~Dx11DepthStencilView()
{
}