//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11UnorderedAccessView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11UnorderedAccessView::Dx11UnorderedAccessView( UnorderedAccessViewComPtr pView )
{
	m_pUnorderedAccessView = pView;
}
//--------------------------------------------------------------------------------
Dx11UnorderedAccessView::~Dx11UnorderedAccessView()
{
}
//--------------------------------------------------------------------------------
