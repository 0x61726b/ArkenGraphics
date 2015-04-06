//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkLight.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkLight.h"
#include "Scene.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkLight::ArkLight()
	:
	Parameters(),
	m_DiffuseWriter( nullptr ),
	m_AmbientWriter( nullptr ),
	m_SpecularWriter( nullptr )
{
	m_DiffuseWriter = Parameters.SetVectorParameter( L"Id", DirectX::XMVectorSet( 0.5f, 0.5f, 0.5f, 1.0f ) );
	m_AmbientWriter = Parameters.SetVectorParameter( L"Ia", DirectX::XMVectorSet( 0.25f, 0.25f, 0.25f, 0.25f ) );
	m_SpecularWriter = Parameters.SetVectorParameter( L"Is", DirectX::XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f ) );

}
ArkLight::~ArkLight()
{
}
//--------------------------------------------------------------------------------
void ArkLight::SetDiffuse( const DirectX::XMVECTOR& color )
{
	m_DiffuseWriter->SetValue( color );
}
//--------------------------------------------------------------------------------
void ArkLight::SetAmbient( const DirectX::XMVECTOR& color )
{
	m_AmbientWriter->SetValue( color );
}
//--------------------------------------------------------------------------------
void ArkLight::SetSpecular( const DirectX::XMVECTOR& color )
{
	m_SpecularWriter->SetValue( color );
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkLight::GetDiffuse( ) const
{
	return( m_DiffuseWriter->GetValue() );
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkLight::GetAmbient( ) const
{
	return( m_AmbientWriter->GetValue() );
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkLight::GetSpecular( ) const
{
	return( m_SpecularWriter->GetValue() );
}
//--------------------------------------------------------------------------------

