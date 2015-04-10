//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterManager11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSamplerParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkSamplerParameter11::ArkSamplerParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_iSampler[i] = -1;
}
//--------------------------------------------------------------------------------
ArkSamplerParameter11::ArkSamplerParameter11( ArkSamplerParameter11& copy )
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_iSampler[i] = copy.m_iSampler[i];
}
//--------------------------------------------------------------------------------
ArkSamplerParameter11::~ArkSamplerParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkSamplerParameter11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	m_iSampler[threadID] = *reinterpret_cast<int*>( pData );
}
//--------------------------------------------------------------------------------
//void SamplerParameterDX11::ResetParameterData( void* pData, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( m_iSampler[threadID] == *reinterpret_cast<int*>( pData ) ) {
//		m_iSampler[threadID] = -1;
//	}
//}
//--------------------------------------------------------------------------------
const ArkParamType ArkSamplerParameter11::GetParameterType()
{
	return( SAMPLER );
}
//--------------------------------------------------------------------------------
int ArkSamplerParameter11::GetIndex( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_iSampler[threadID] );
}
//--------------------------------------------------------------------------------
//void SamplerParameterDX11::UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( pParameter )
//	{
//		if ( ( pParameter->GetParameterType() == SAMPLER ) && ( pParameter->GetName() == this->GetName() ) )
//		{
//			SamplerParameterDX11* pBuffer = (SamplerParameterDX11*)pParameter;
//			m_iSampler[threadID] = pBuffer->GetIndex( threadID );
//		}
//	}
//}
//--------------------------------------------------------------------------------