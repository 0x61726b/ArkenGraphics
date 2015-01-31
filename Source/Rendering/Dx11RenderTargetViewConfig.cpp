//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11RenderTargetViewConfig.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11RenderTargetViewConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11RenderTargetViewConfig::Dx11RenderTargetViewConfig()
{
    SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11RenderTargetViewConfig::~Dx11RenderTargetViewConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetDefaults()
{
    // Set the state to the default configuration.  These are the D3D11 default
    // values as well.

    m_State.Format = DXGI_FORMAT_UNKNOWN;
    m_State.ViewDimension = D3D11_RTV_DIMENSION_UNKNOWN;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetFormat( DXGI_FORMAT state )
{
    m_State.Format = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetViewDimensions( D3D11_RTV_DIMENSION state )
{
    m_State.ViewDimension = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetBuffer( D3D11_BUFFER_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
    m_State.Buffer = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture1D( D3D11_TEX1D_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
    m_State.Texture1D = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture1DArray( D3D11_TEX1D_ARRAY_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
    m_State.Texture1DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture2D( D3D11_TEX2D_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    m_State.Texture2D = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture2DArray( D3D11_TEX2D_ARRAY_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    m_State.Texture2DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture2DMS( D3D11_TEX2DMS_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    m_State.Texture2DMS = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture2DMSArray( D3D11_TEX2DMS_ARRAY_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
    m_State.Texture2DMSArray = state;
}
//--------------------------------------------------------------------------------
void Dx11RenderTargetViewConfig::SetTexture3D( D3D11_TEX3D_RTV state )
{
    m_State.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
    m_State.Texture3D = state;
}
//--------------------------------------------------------------------------------
D3D11_RENDER_TARGET_VIEW_DESC& Dx11RenderTargetViewConfig::GetRTVDesc()
{
    return( m_State );
}
//--------------------------------------------------------------------------------

