//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderResourceParameter11.h
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkShaderResourceParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkShaderResourceParameter11::ArkShaderResourceParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_iShaderResourceView[i] = 0;
}
//--------------------------------------------------------------------------------
ArkShaderResourceParameter11::ArkShaderResourceParameter11( ArkShaderResourceParameter11& copy )
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_iShaderResourceView[i] = copy.m_iShaderResourceView[i];
}
//--------------------------------------------------------------------------------
ArkShaderResourceParameter11::~ArkShaderResourceParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkShaderResourceParameter11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	m_iShaderResourceView[threadID] = *reinterpret_cast<int*>( pData );
}
//--------------------------------------------------------------------------------
//void ShaderResourceParameterDX11::ResetParameterData( void* pData, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( m_iShaderResourceView[threadID] == *reinterpret_cast<int*>( pData ) ) {
//		m_iShaderResourceView[threadID] = -1;
//	}
//}
//--------------------------------------------------------------------------------
const ArkParamType ArkShaderResourceParameter11::GetParameterType()
{
	return( SHADER_RESOURCE );
}
//--------------------------------------------------------------------------------
int ArkShaderResourceParameter11::GetIndex( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_iShaderResourceView[threadID] );
}
//--------------------------------------------------------------------------------
//void ShaderResourceParameterDX11::UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( pParameter )
//	{
//		if ( ( pParameter->GetParameterType() == SHADER_RESOURCE ) && ( pParameter->GetName() == this->GetName() ) )
//		{
//			ShaderResourceParameterDX11* pBuffer = (ShaderResourceParameterDX11*)pParameter;
//			m_iShaderResourceView[threadID] = pBuffer->GetIndex( threadID );
//		}
//	}
//}
//--------------------------------------------------------------------------------