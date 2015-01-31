//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ShaderResourceView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11ShaderResourceView::Dx11ShaderResourceView( ShaderResourceViewComPtr pView )
{
	m_pShaderResourceView = pView;
}
//--------------------------------------------------------------------------------
Dx11ShaderResourceView::~Dx11ShaderResourceView()
{
}
//--------------------------------------------------------------------------------
ID3D11ShaderResourceView* Dx11ShaderResourceView::GetSRV()
{
	return m_pShaderResourceView.Get();
}
//--------------------------------------------------------------------------------

