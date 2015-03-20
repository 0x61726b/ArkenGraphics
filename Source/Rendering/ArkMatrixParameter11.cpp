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
	m_Matrix = DirectX::XMMatrixIdentity();
}
//--------------------------------------------------------------------------------
ArkMatrixParameter11::ArkMatrixParameter11(ArkMatrixParameter11& copy)
{
	m_Matrix = copy.m_Matrix;
}
//--------------------------------------------------------------------------------
ArkMatrixParameter11::~ArkMatrixParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkMatrixParameter11::SetParameterData(void* pData,unsigned int thread)
{
	if(0 != memcpy(pData,&m_Matrix,sizeof(DirectX::XMMATRIX)))
	{
		m_Matrix = *reinterpret_cast<DirectX::XMMATRIX*>(pData);
	}
}
//--------------------------------------------------------------------------------
const ArkParamType ArkMatrixParameter11::GetParameterType()
{
	return ArkParamType::MATRIX;
}
//--------------------------------------------------------------------------------
DirectX::XMMATRIX ArkMatrixParameter11::GetMatrix()
{
	return m_Matrix;
}
//--------------------------------------------------------------------------------
void ArkMatrixParameter11::SetMatrix( DirectX::XMMATRIX& v )
{
	m_Matrix = v;
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
