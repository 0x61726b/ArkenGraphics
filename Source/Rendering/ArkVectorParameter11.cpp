//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVectorParameter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkVectorParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkVectorParameter11::ArkVectorParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_Vector[i] = DirectX::XMVectorZero();
	
}
//--------------------------------------------------------------------------------
ArkVectorParameter11::ArkVectorParameter11(ArkVectorParameter11& copy)
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_Vector[i] = copy.m_Vector[i];
}
//--------------------------------------------------------------------------------
ArkVectorParameter11::~ArkVectorParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkVectorParameter11::SetParameterData(void* pData,unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	if ( 0 != memcmp( pData, &(m_Vector[threadID]), sizeof(DirectX::XMVECTOR) ) ) {
		m_auiValueID[threadID]++;
		m_Vector[threadID] =  *reinterpret_cast<DirectX::XMVECTOR*>( pData );
	}
}
//--------------------------------------------------------------------------------
const ArkParamType ArkVectorParameter11::GetParameterType()
{
	return ArkParamType::VECTOR;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkVectorParameter11::GetVector(unsigned int threadID)
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_Vector[threadID] );
}
//--------------------------------------------------------------------------------
void ArkVectorParameter11::SetVector( DirectX::XMVECTOR v,unsigned int threadID)
{
	if ( !DirectX::XMVector4Equal(v,m_Vector[threadID] ))
	{
		m_auiValueID[threadID]++;
		m_Vector[threadID] = v;
	}
}