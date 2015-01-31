//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//SwapChain.h
//--------------------------------------------------------------------------------
#ifndef __Dx11SwapChain_h__
#define __Dx11SwapChain_h__
//--------------------------------------------------------------------------------
#include "D3D11Renderer.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11SwapChain
	{
	public:
		explicit Dx11SwapChain( Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain, ResourcePtr resource );
		virtual ~Dx11SwapChain();

		IDXGISwapChain* GetSwapChain();
	protected:
		Microsoft::WRL::ComPtr<IDXGISwapChain>  m_pSwapChain;
		ResourcePtr								m_Resource;

		friend D3D11Renderer;
	};
};

//--------------------------------------------------------------------------------
#endif