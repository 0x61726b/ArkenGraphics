//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexBuffer11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkVertexBuffer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkVertexBuffer11::ArkVertexBuffer11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer )
{
	m_pBuffer = pBuffer;
}
//--------------------------------------------------------------------------------
ArkVertexBuffer11::~ArkVertexBuffer11()
{
	// Buffer is released in the BufferDX11 destructor
}
//--------------------------------------------------------------------------------
ResourceType ArkVertexBuffer11::GetType()
{
	return( RT_VERTEXBUFFER );
}
//--------------------------------------------------------------------------------
void ArkVertexBuffer11::SetVertexSize( int size )
{
	m_iVertexSize = size;
}
//--------------------------------------------------------------------------------
void ArkVertexBuffer11::SetVertexCount( int count )
{
	m_iVertexCount = count;
}
//--------------------------------------------------------------------------------