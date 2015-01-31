//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11ShaderResourceViewConfig.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11ShaderResourceViewConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11ShaderResourceViewConfig::Dx11ShaderResourceViewConfig()
{
    SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11ShaderResourceViewConfig::~Dx11ShaderResourceViewConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetDefaults()
{
    // Set the state to the default configuration.  These are the D3D11 default
    // values as well.
    
    m_State.Format = DXGI_FORMAT_UNKNOWN;
    m_State.ViewDimension = D3D11_SRV_DIMENSION_UNKNOWN;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetFormat( DXGI_FORMAT state )
{
    m_State.Format = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetViewDimensions( D3D11_SRV_DIMENSION state )
{
    m_State.ViewDimension = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetBuffer( D3D11_BUFFER_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    m_State.Buffer = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetBufferEx( D3D11_BUFFEREX_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    m_State.BufferEx = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture1D( D3D11_TEX1D_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    m_State.Texture1D = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture1DArray( D3D11_TEX1D_ARRAY_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
    m_State.Texture1DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture2D( D3D11_TEX2D_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    m_State.Texture2D = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture2DArray( D3D11_TEX2D_ARRAY_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    m_State.Texture2DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture2DMS( D3D11_TEX2DMS_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    m_State.Texture2DMS = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture2DMSArray( D3D11_TEX2DMS_ARRAY_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
    m_State.Texture2DMSArray = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTexture3D( D3D11_TEX3D_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
    m_State.Texture3D = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTextureCube( D3D11_TEXCUBE_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    m_State.TextureCube = state;
}
//--------------------------------------------------------------------------------
void Dx11ShaderResourceViewConfig::SetTextureCubeArray( D3D11_TEXCUBE_ARRAY_SRV state )
{
    m_State.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
    m_State.TextureCubeArray = state;
}
//--------------------------------------------------------------------------------
D3D11_SHADER_RESOURCE_VIEW_DESC& Dx11ShaderResourceViewConfig::GetSRVDesc()
{
    return( m_State );
}
//--------------------------------------------------------------------------------
