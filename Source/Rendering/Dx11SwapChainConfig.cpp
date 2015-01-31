//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11SwapChainConfig.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11SwapChainConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11SwapChainConfig::Dx11SwapChainConfig()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11SwapChainConfig::~Dx11SwapChainConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	m_State.BufferDesc.Width = 1;
	m_State.BufferDesc.Height = 1;
    m_State.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	m_State.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    m_State.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_State.BufferDesc.RefreshRate.Numerator = 60;
	m_State.BufferDesc.RefreshRate.Denominator = 1;

	m_State.SampleDesc.Count = 1;
	m_State.SampleDesc.Quality = 0;

	m_State.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_State.BufferCount = 2;
	m_State.OutputWindow = 0;
	m_State.Windowed = true;
	m_State.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// If you are creating a full screen swap chain, you may want to include the 
	// DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH flag in the flags below, which will
	// override the default DXGI behavior.  See here for more details:
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ee417025%28v=vs.85%29.aspx

	m_State.Flags = 0; 
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetWidth( UINT width )
{
	m_State.BufferDesc.Width = width;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetHeight( UINT height )
{
	m_State.BufferDesc.Height = height;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetRefreshRateNumerator( UINT numerator )
{
	m_State.BufferDesc.RefreshRate.Numerator = numerator;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetRefreshRateDenominator( UINT denominator )
{
	m_State.BufferDesc.RefreshRate.Denominator = denominator;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetFormat( DXGI_FORMAT Format )
{
	m_State.BufferDesc.Format = Format;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetScanlineOrder( DXGI_MODE_SCANLINE_ORDER ScanlineOrdering )
{
	m_State.BufferDesc.ScanlineOrdering = ScanlineOrdering;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetScaling( DXGI_MODE_SCALING Scaling )
{
	m_State.BufferDesc.Scaling = Scaling;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetBufferDesc( DXGI_MODE_DESC BufferDesc )
{
	m_State.BufferDesc = BufferDesc;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetSampleDesc( DXGI_SAMPLE_DESC SampleDesc )
{
	m_State.SampleDesc = SampleDesc;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetBufferUsage( DXGI_USAGE BufferUsage )
{
	m_State.BufferUsage = BufferUsage;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetBufferCount( UINT BufferCount )
{
	m_State.BufferCount = BufferCount;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetOutputWindow( HWND OutputWindow )
{
	m_State.OutputWindow = OutputWindow;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetWindowed( bool Windowed )
{
	m_State.Windowed = Windowed;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetSwapEffect( DXGI_SWAP_EFFECT SwapEffect )
{
	m_State.SwapEffect = SwapEffect;
}
//--------------------------------------------------------------------------------
void Dx11SwapChainConfig::SetFlags( UINT Flags )
{
	m_State.Flags = Flags;
}
//--------------------------------------------------------------------------------
DXGI_SWAP_CHAIN_DESC Dx11SwapChainConfig::GetSwapChainDesc()
{
	return( m_State );
}
//--------------------------------------------------------------------------------
