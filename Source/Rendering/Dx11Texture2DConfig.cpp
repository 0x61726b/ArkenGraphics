//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11Texture2DConfig.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11Texture2DConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11Texture2DConfig::Dx11Texture2DConfig()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
Dx11Texture2DConfig::~Dx11Texture2DConfig()
{
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

    m_State.Width = 1;
	m_State.Height = 1;
    m_State.MipLevels = 1;
    m_State.ArraySize = 1;
    m_State.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_State.SampleDesc.Count = 1;
	m_State.SampleDesc.Quality = 0;
    m_State.Usage = D3D11_USAGE_DEFAULT;
    m_State.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    m_State.CPUAccessFlags = 0;
    m_State.MiscFlags = 0;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetDepthBuffer(UINT width, UINT height)
{
	m_State.Width = width;
	m_State.Height = height;
	m_State.MipLevels = 1;
	m_State.ArraySize = 1;
	m_State.Format = DXGI_FORMAT_D32_FLOAT;
	m_State.SampleDesc.Count = 1;
	m_State.SampleDesc.Quality = 0;
	m_State.Usage = D3D11_USAGE_DEFAULT;
	m_State.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_State.CPUAccessFlags = 0;
	m_State.MiscFlags = 0;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetColorBuffer(UINT width, UINT height)
{
	m_State.Width = width;
	m_State.Height = height;
	m_State.MipLevels = 1;
	m_State.ArraySize = 1;
	m_State.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_State.SampleDesc.Count = 1;
	m_State.SampleDesc.Quality = 0;
	m_State.Usage = D3D11_USAGE_DEFAULT;
	m_State.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_State.CPUAccessFlags = 0;
	m_State.MiscFlags = 0;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetWidth( UINT state )
{
	m_State.Width = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetHeight( UINT state )
{
	m_State.Height = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetMipLevels( UINT state )
{
	m_State.MipLevels = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetArraySize( UINT state )
{
	m_State.ArraySize = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetFormat( DXGI_FORMAT state )
{
	m_State.Format = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetSampleDesc( DXGI_SAMPLE_DESC state )
{
	m_State.SampleDesc = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetUsage( D3D11_USAGE state ) 
{
	m_State.Usage = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetBindFlags( UINT state )
{
	m_State.BindFlags = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetCPUAccessFlags( UINT state )
{
	m_State.CPUAccessFlags = state;
}
//--------------------------------------------------------------------------------
void Dx11Texture2DConfig::SetMiscFlags( UINT state )
{
	m_State.MiscFlags = state;
}
//--------------------------------------------------------------------------------
D3D11_TEXTURE2D_DESC Dx11Texture2DConfig::GetTextureDesc()
{
	return( m_State );
}
//--------------------------------------------------------------------------------
