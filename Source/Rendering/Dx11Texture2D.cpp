//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ResourceProxy.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Dx11Texture2D.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Dx11Texture2D::Dx11Texture2D( Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex )
{
	m_pTexture = pTex;

	ZeroMemory( &m_DesiredDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
}
//--------------------------------------------------------------------------------
Dx11Texture2D::~Dx11Texture2D()
{
}
//--------------------------------------------------------------------------------
ResourceType Dx11Texture2D::GetType()
{
	return( RT_TEXTURE2D );
}
//--------------------------------------------------------------------------------
D3D11_TEXTURE2D_DESC Dx11Texture2D::GetActualDescription()
{
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	if ( m_pTexture )
		m_pTexture->GetDesc( &m_ActualDesc );

	return( m_ActualDesc );
}
//--------------------------------------------------------------------------------
D3D11_TEXTURE2D_DESC Dx11Texture2D::GetDesiredDescription()
{
	return( m_DesiredDesc );
}
//--------------------------------------------------------------------------------
void Dx11Texture2D::SetDesiredDescription( D3D11_TEXTURE2D_DESC description )
{
	m_DesiredDesc = description;
}
//--------------------------------------------------------------------------------
ID3D11Resource* Dx11Texture2D::GetResource()
{
	return( m_pTexture.Get() );
}
//--------------------------------------------------------------------------------
UINT Dx11Texture2D::GetEvictionPriority()
{
	UINT priority = 0;

	if ( m_pTexture )
		priority = m_pTexture->GetEvictionPriority();

	return( priority );
}
//--------------------------------------------------------------------------------
void Dx11Texture2D::SetEvictionPriority( UINT EvictionPriority )
{
	if ( m_pTexture )
		m_pTexture->SetEvictionPriority( EvictionPriority );
}
//--------------------------------------------------------------------------------
