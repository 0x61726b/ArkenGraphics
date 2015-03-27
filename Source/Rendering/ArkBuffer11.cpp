//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBuffer11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkBuffer11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkBuffer11::ArkBuffer11()
{
	ZeroMemory( &m_DesiredDesc, sizeof( D3D11_BUFFER_DESC ) );
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_BUFFER_DESC ) );
}
//--------------------------------------------------------------------------------
ArkBuffer11::~ArkBuffer11()
{
	ArkLog::Get( LogType::Renderer ).Output(L"Releasing the Buffer!");
}
//--------------------------------------------------------------------------------
D3D11_BUFFER_DESC ArkBuffer11::GetActualDescription()
{
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_BUFFER_DESC ) );

	if ( m_pBuffer )
		m_pBuffer->GetDesc( &m_ActualDesc );

	return( m_ActualDesc );
}
//--------------------------------------------------------------------------------
D3D11_BUFFER_DESC ArkBuffer11::GetDesiredDescription()
{
	return( m_DesiredDesc );
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetDesiredDescription( D3D11_BUFFER_DESC desc )
{
	m_DesiredDesc = desc;
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetByteWidth()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.ByteWidth );
}
//--------------------------------------------------------------------------------
D3D11_USAGE ArkBuffer11::GetUsage()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.Usage );
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetBindFlags()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.BindFlags );
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetCPUAccessFlags()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.CPUAccessFlags );
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetMiscFlags()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.MiscFlags );
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetStructureByteStride()
{
	D3D11_BUFFER_DESC description = GetActualDescription();

	return( description.StructureByteStride );
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetByteWidth( UINT width )
{
	m_DesiredDesc.ByteWidth = width;
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetUsage( D3D11_USAGE usage )
{
	m_DesiredDesc.Usage = usage;
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetBindFlags( UINT flags )
{
	m_DesiredDesc.BindFlags = flags;
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetCPUAccessFlags( UINT flags )
{
	m_DesiredDesc.CPUAccessFlags = flags;
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetMiscFlags( UINT flags )
{
	m_DesiredDesc.MiscFlags = flags;
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetStructureByteStride( UINT stride )
{
	m_DesiredDesc.StructureByteStride = stride;
}
//--------------------------------------------------------------------------------
void* ArkBuffer11::Map()
{
	return( 0 );
}
//--------------------------------------------------------------------------------
void ArkBuffer11::UnMap()
{
}
//--------------------------------------------------------------------------------
ID3D11Resource*	ArkBuffer11::GetResource()
{
	return( m_pBuffer.Get() );
}
//--------------------------------------------------------------------------------
UINT ArkBuffer11::GetEvictionPriority()
{
	UINT priority = 0;

	if ( m_pBuffer )
		priority = m_pBuffer->GetEvictionPriority();

	return( priority );
}
//--------------------------------------------------------------------------------
void ArkBuffer11::SetEvictionPriority( UINT EvictionPriority )
{
	if ( m_pBuffer )
		m_pBuffer->SetEvictionPriority( EvictionPriority );
}
//--------------------------------------------------------------------------------