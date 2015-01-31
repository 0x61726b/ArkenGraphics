//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11SwapChain.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11SwapChain.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11SwapChain::Dx11SwapChain( Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain, ResourcePtr resource )
{
	m_pSwapChain = pSwapChain;
	m_Resource = resource;
}
//--------------------------------------------------------------------------------
Dx11SwapChain::~Dx11SwapChain()
{
}
//--------------------------------------------------------------------------------
IDXGISwapChain* Dx11SwapChain::GetSwapChain()
{
	return( m_pSwapChain.Get() );
}
//--------------------------------------------------------------------------------
