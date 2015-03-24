//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixParameter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMatrixParameter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMatrixParameter11::ArkMatrixParameter11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_Matrix[i] = DirectX::XMMatrixIdentity();
}
//--------------------------------------------------------------------------------
ArkMatrixParameter11::ArkMatrixParameter11(ArkMatrixParameter11& copy)
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_Matrix[i] = copy.m_Matrix[i];
}
//--------------------------------------------------------------------------------
ArkMatrixParameter11::~ArkMatrixParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkMatrixParameter11::SetParameterData(void* pData,unsigned int thread)
{
	assert( thread >= 0 );
	assert( thread < NUM_THREADS+1 );

	if ( 0 != memcmp( pData, &(m_Matrix[thread]), sizeof(DirectX::XMMATRIX) ) )
	{
		m_auiValueID[thread]++;
		m_Matrix[thread] = *reinterpret_cast<DirectX::XMMATRIX*>(pData);
	}
}
//--------------------------------------------------------------------------------
const ArkParamType ArkMatrixParameter11::GetParameterType()
{
	return ArkParamType::MATRIX;
}
//--------------------------------------------------------------------------------
DirectX::XMMATRIX ArkMatrixParameter11::GetMatrix(unsigned int thread)
{
	return m_Matrix[thread];
}
//--------------------------------------------------------------------------------
void* ArkMatrixParameter11::operator new(size_t i)
{
	return _mm_malloc(i,16);
}
//--------------------------------------------------------------------------------
void ArkMatrixParameter11::operator delete(void* p)
{
	_mm_free(p);
}
