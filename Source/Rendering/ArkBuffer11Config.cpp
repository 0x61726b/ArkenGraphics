//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBuffer11Config.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkBuffer11Config.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkBuffer11Config::ArkBuffer11Config()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
ArkBuffer11Config::~ArkBuffer11Config()
{
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	m_State.ByteWidth = 1;
    m_State.Usage = D3D11_USAGE_DEFAULT;
    m_State.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_State.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_State.MiscFlags = 0;
    m_State.StructureByteStride = 0;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultConstantBuffer( UINT size, bool dynamic )
{
	// Create the settings for a constant buffer.  This includes setting the 
	// constant buffer flag, allowing the CPU write access, and a dynamic usage.
	// Additional flags may be set as needed.

	m_State.ByteWidth = size;
    m_State.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_State.MiscFlags = 0;
    m_State.StructureByteStride = 0;

	if ( dynamic )
	{
		m_State.Usage = D3D11_USAGE_DYNAMIC;
		m_State.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_State.Usage = D3D11_USAGE_IMMUTABLE;
		m_State.CPUAccessFlags = 0;
	}
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultVertexBuffer( UINT size, bool dynamic )
{
	// Create the settings for a vertex buffer.  This includes the setting the
	// vertex buffer flag, 

	m_State.ByteWidth = size;
    m_State.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_State.MiscFlags = 0;
    m_State.StructureByteStride = 0;

	if ( dynamic )
	{
		m_State.Usage = D3D11_USAGE_DYNAMIC;
		m_State.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_State.Usage = D3D11_USAGE_IMMUTABLE;
		m_State.CPUAccessFlags = 0;
	}
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultIndexBuffer( UINT size, bool dynamic )
{
	m_State.ByteWidth = size;
    m_State.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_State.MiscFlags = 0;
    m_State.StructureByteStride = 0;

	if ( dynamic )
	{
		m_State.Usage = D3D11_USAGE_DYNAMIC;
		m_State.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_State.Usage = D3D11_USAGE_IMMUTABLE;
		m_State.CPUAccessFlags = 0;
	}
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultStructuredBuffer( UINT size, UINT structsize )
{
	m_State.ByteWidth = size * structsize;
    m_State.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    m_State.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    m_State.StructureByteStride = structsize;

	m_State.Usage = D3D11_USAGE_DEFAULT;
	m_State.CPUAccessFlags = 0;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultByteAddressBuffer( UINT size )
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	// TODO: These parameters need to be updated to reflect the byte address buffer!
	m_State.ByteWidth = size;
    m_State.Usage = D3D11_USAGE_DEFAULT;
    m_State.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    m_State.CPUAccessFlags = 0;
    m_State.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    m_State.StructureByteStride = 0;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultIndirectArgsBuffer( UINT size )
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	m_State.ByteWidth = size;
    m_State.Usage = D3D11_USAGE_DEFAULT;
    m_State.BindFlags = 0;
    m_State.CPUAccessFlags = 0;
    m_State.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
    m_State.StructureByteStride = 0;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetDefaultStagingBuffer( UINT size )
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	m_State.ByteWidth = size;
    m_State.Usage = D3D11_USAGE_STAGING;
    m_State.BindFlags = 0;
    m_State.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    m_State.MiscFlags = 0;
    m_State.StructureByteStride = 0;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetByteWidth( UINT state )
{
	m_State.ByteWidth = state;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetUsage( D3D11_USAGE state )
{
	m_State.Usage = state;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetBindFlags( UINT state )
{
	m_State.BindFlags = state;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetCPUAccessFlags( UINT state )
{
	m_State.CPUAccessFlags = state;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetMiscFlags( UINT state )
{
	m_State.MiscFlags = state;
}
//--------------------------------------------------------------------------------
void ArkBuffer11Config::SetStructureByteStride( UINT state )
{
	m_State.StructureByteStride = state;
}
//--------------------------------------------------------------------------------
D3D11_BUFFER_DESC ArkBuffer11Config::GetBufferDesc()
{
	return( m_State );
}
//--------------------------------------------------------------------------------

