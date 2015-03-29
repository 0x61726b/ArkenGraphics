//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixParameter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMatrixArrayParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11::ArkMatrixArrayParameter11(int count)
{
	if ( count < 1 )
		count = 1;

	m_iMatrixCount = count; 

	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_pMatrices[i] = new DirectX::XMMATRIX[count];
	
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11::ArkMatrixArrayParameter11(ArkMatrixArrayParameter11& copy)
{
	if ( this->m_iMatrixCount != copy.m_iMatrixCount )
	{
		for ( int i = 0; i <= NUM_THREADS; i++ )
		{
			delete [] m_pMatrices[i];
			m_pMatrices[i] = new DirectX::XMMATRIX[copy.m_iMatrixCount];
		}
		m_iMatrixCount = copy.m_iMatrixCount;
	}

	for ( int i = 0; i <= NUM_THREADS; i++ )
		memcpy( m_pMatrices[i], copy.m_pMatrices[i], m_iMatrixCount * sizeof( DirectX::XMMATRIX) );
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11& ArkMatrixArrayParameter11::operator=( ArkMatrixArrayParameter11& parameter )
{
	if ( this->m_iMatrixCount != parameter.m_iMatrixCount )
	{
		for ( int i = 0; i <= NUM_THREADS; i++ )
		{
			delete [] m_pMatrices[i];
			m_pMatrices[i] = new DirectX::XMMATRIX[parameter.m_iMatrixCount];
		}
		m_iMatrixCount = parameter.m_iMatrixCount;
	}

	for ( int i = 0; i <= NUM_THREADS; i++ )
		memcpy( m_pMatrices[i], parameter.m_pMatrices[i], m_iMatrixCount * sizeof( DirectX::XMMATRIX) );

   return *this;  // Assignment operator returns left side.
}
ArkMatrixArrayParameter11::~ArkMatrixArrayParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		delete [] m_pMatrices[i];
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameter11::SetParameterData(void* pData,unsigned int thread)
{
	assert( thread >= 0 );
	assert( thread < NUM_THREADS+1 );

	// TODO: This isn't very safe - the caller could supply less than the correct 
	//       amount of matrices...  I need a better way to set this parameter data.

	if ( 0 != memcmp( pData, &(m_pMatrices[thread]), m_iMatrixCount * sizeof( DirectX::XMMATRIX ) ) ) {
		m_auiValueID[thread]++;
		memcpy( m_pMatrices[thread], pData, m_iMatrixCount * sizeof( DirectX::XMMATRIX ) );
	}
	
}
//--------------------------------------------------------------------------------
const ArkParamType ArkMatrixArrayParameter11::GetParameterType()
{
	return ArkParamType::MATRIX_ARRAY;
}
//--------------------------------------------------------------------------------
DirectX::XMMATRIX* ArkMatrixArrayParameter11::GetMatrices(unsigned int threadID)
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_pMatrices[threadID] );
}
//--------------------------------------------------------------------------------
int ArkMatrixArrayParameter11::GetMatrixCount()
{
	return m_iMatrixCount;
}
//--------------------------------------------------------------------------------


