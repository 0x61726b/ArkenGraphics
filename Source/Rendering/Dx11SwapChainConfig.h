//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11SwapChainConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11SwapChainConfig_h__
#define __Dx11SwapChainConfig_h__
//--------------------------------------------------------------------------------
#include "D3D11Renderer.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Dx11SwapChainConfig
	{
	public:
		Dx11SwapChainConfig();
		virtual ~Dx11SwapChainConfig();

		void SetDefaults();

		void SetWidth(UINT width);
		void SetHeight(UINT height);
		void SetRefreshRateNumerator(UINT numerator);
		void SetRefreshRateDenominator(UINT denominator);
		void SetFormat(DXGI_FORMAT Format);
		void SetScanlineOrder(DXGI_MODE_SCANLINE_ORDER ScanlineOrdering);
		void SetScaling(DXGI_MODE_SCALING Scaling);


		void SetBufferDesc(DXGI_MODE_DESC BufferDesc);
		void SetSampleDesc(DXGI_SAMPLE_DESC SampleDesc);
		void SetBufferUsage(DXGI_USAGE BufferUsage);
		void SetBufferCount(UINT BufferCount);
		void SetOutputWindow(HWND OutputWindow);
		void SetWindowed(bool Windowed);
		void SetSwapEffect(DXGI_SWAP_EFFECT SwapEffect);
		void SetFlags(UINT Flags);

		DXGI_SWAP_CHAIN_DESC GetSwapChainDesc();

	protected:
		DXGI_SWAP_CHAIN_DESC 		m_State;

		friend D3D11Renderer;
	};

};
//--------------------------------------------------------------------------------
#endif