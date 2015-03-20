//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexBuffer11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkIndexBuffer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkIndexBuffer11::ArkIndexBuffer11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer )
	:m_iIndexSize(0),
	m_iIndexCount(0)
{
	m_pBuffer = pBuffer;
}
//--------------------------------------------------------------------------------
ArkIndexBuffer11::~ArkIndexBuffer11()
{
	// Buffer is released in the BufferDX11 destructor
}
//--------------------------------------------------------------------------------
ResourceType ArkIndexBuffer11::GetType()
{
	return( RT_INDEXBUFFER );
}
//--------------------------------------------------------------------------------
void ArkIndexBuffer11::SetIndexSize( int size )
{
	m_iIndexSize = size;
}
//--------------------------------------------------------------------------------
void ArkIndexBuffer11::SetIndexCount( int count )
{
	m_iIndexCount = count;
}
//--------------------------------------------------------------------------------