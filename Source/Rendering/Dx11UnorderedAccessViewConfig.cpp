//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11UnorderedAccessViewConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11UnorderedAccessViewConfig::Dx11UnorderedAccessViewConfig()
{
    SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11UnorderedAccessViewConfig::~Dx11UnorderedAccessViewConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetDefaults()
{
    // Set the state to the default configuration.  These are the D3D11 default
    // values as well.

    m_State.Format = DXGI_FORMAT_UNKNOWN;
    m_State.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetFormat( DXGI_FORMAT state )
{
    m_State.Format = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetViewDimensions( D3D11_UAV_DIMENSION state )
{
    m_State.ViewDimension = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetBuffer( D3D11_BUFFER_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    m_State.Buffer = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetTexture1D( D3D11_TEX1D_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
    m_State.Texture1D = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetTexture1DArray( D3D11_TEX1D_ARRAY_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
    m_State.Texture1DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetTexture2D( D3D11_TEX2D_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    m_State.Texture2D = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetTexture2DArray( D3D11_TEX2D_ARRAY_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
    m_State.Texture2DArray = state;
}
//--------------------------------------------------------------------------------
void Dx11UnorderedAccessViewConfig::SetTexture3D( D3D11_TEX3D_UAV state )
{
    m_State.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
    m_State.Texture3D = state;
}
//--------------------------------------------------------------------------------
D3D11_UNORDERED_ACCESS_VIEW_DESC& Dx11UnorderedAccessViewConfig::GetUAVDesc()
{
    return( m_State );
}
//--------------------------------------------------------------------------------
