//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkUnorderedAccessParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkUnorderedAccessParameter11::ArkUnorderedAccessParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
	{
		m_ParameterData[i].m_iUnorderedAccessView = 0;
		m_ParameterData[i].m_iInitialCount = -1;
	}
}
//--------------------------------------------------------------------------------
ArkUnorderedAccessParameter11::ArkUnorderedAccessParameter11( ArkUnorderedAccessParameter11& copy )
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
	{
		m_ParameterData[i].m_iUnorderedAccessView = copy.m_ParameterData[i].m_iUnorderedAccessView;
		m_ParameterData[i].m_iInitialCount = copy.m_ParameterData[i].m_iInitialCount;
	}
}
//--------------------------------------------------------------------------------
ArkUnorderedAccessParameter11::~ArkUnorderedAccessParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkUnorderedAccessParameter11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	m_ParameterData[threadID] = *reinterpret_cast<UAVParameterData*>( pData );
}
//--------------------------------------------------------------------------------
//void UnorderedAccessParameterDX11::ResetParameterData( void* pData, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( m_ParameterData[threadID].m_iUnorderedAccessView 
//		== (*reinterpret_cast<UAVParameterData*>( pData )).m_iUnorderedAccessView ) {
//
//		m_ParameterData[threadID].m_iUnorderedAccessView = -1;
//		m_ParameterData[threadID].m_iInitialCount = -1;
//	}
//}
//--------------------------------------------------------------------------------
const ArkParamType ArkUnorderedAccessParameter11::GetParameterType()
{
	return( UOA );
}
//--------------------------------------------------------------------------------
int ArkUnorderedAccessParameter11::GetIndex( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_ParameterData[threadID].m_iUnorderedAccessView );
}
//--------------------------------------------------------------------------------
unsigned int ArkUnorderedAccessParameter11::GetInitialCount( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_ParameterData[threadID].m_iInitialCount );
}
//--------------------------------------------------------------------------------
//void UnorderedAccessParameterDX11::UpdateValue( RenderParameterDX11* pParameter, unsigned int threadID )
//{
//	assert( threadID >= 0 );
//	assert( threadID < NUM_THREADS+1 );
//
//	if ( pParameter )
//	{
//		if ( ( pParameter->GetParameterType() == UNORDERED_ACCESS ) && ( pParameter->GetName() == this->GetName() ) )
//		{
//			UnorderedAccessParameterDX11* pBuffer = (UnorderedAccessParameterDX11*)pParameter;
//			m_ParameterData[threadID].m_iUnorderedAccessView = pBuffer->GetIndex( threadID );
//			m_ParameterData[threadID].m_iInitialCount = pBuffer->GetInitialCount( threadID );
//		}
//	}
//}
//--------------------------------------------------------------------------------