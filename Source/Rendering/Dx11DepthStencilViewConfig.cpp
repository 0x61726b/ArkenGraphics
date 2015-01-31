//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11DepthStencilViewConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11DepthStencilViewConfig::Dx11DepthStencilViewConfig( )
{
	
}
//--------------------------------------------------------------------------------
Dx11DepthStencilViewConfig::~Dx11DepthStencilViewConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	m_State.Flags = 0;
	m_State.Format = DXGI_FORMAT_UNKNOWN;
	m_State.ViewDimension = D3D11_DSV_DIMENSION_UNKNOWN;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetFormat( DXGI_FORMAT state )
{
	m_State.Format = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetViewDimensions( D3D11_DSV_DIMENSION state )
{
	m_State.ViewDimension = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetFlags( UINT state )
{
	m_State.Flags = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture1D( D3D11_TEX1D_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
	m_State.Texture1D = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture1DArray( D3D11_TEX1D_ARRAY_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
	m_State.Texture1DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture2D( D3D11_TEX2D_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_State.Texture2D = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture2DArray( D3D11_TEX2D_ARRAY_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	m_State.Texture2DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture2DMS( D3D11_TEX2DMS_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	m_State.Texture2DMS = state;
}
//--------------------------------------------------------------------------------
void Dx11DepthStencilViewConfig::SetTexture2DMSArray( D3D11_TEX2DMS_ARRAY_DSV state )
{
	m_State.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
	m_State.Texture2DMSArray = state;
}
//--------------------------------------------------------------------------------
D3D11_DEPTH_STENCIL_VIEW_DESC& Dx11DepthStencilViewConfig::GetDSVDesc()
{
	return( m_State );
}
//--------------------------------------------------------------------------------