//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameter11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkConstantBufferParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkConstantBufferParameter11::ArkConstantBufferParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_iCBuffer[i] = -1;
}
//--------------------------------------------------------------------------------
ArkConstantBufferParameter11::ArkConstantBufferParameter11( ArkConstantBufferParameter11& copy )
{
	for ( int i = 0; i <=NUM_THREADS; i++ )
		m_iCBuffer[i] = copy.m_iCBuffer[i];
}
//--------------------------------------------------------------------------------
ArkConstantBufferParameter11::~ArkConstantBufferParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkConstantBufferParameter11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < 1+NUM_THREADS );

	m_iCBuffer[threadID] = *reinterpret_cast<int*>( pData );
}
//--------------------------------------------------------------------------------
const ArkParamType ArkConstantBufferParameter11::GetParameterType()
{
	return( CBUFFER );
}
//--------------------------------------------------------------------------------
int ArkConstantBufferParameter11::GetIndex( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < 1+NUM_THREADS );

	return( m_iCBuffer[threadID] );
}
//--------------------------------------------------------------------------------