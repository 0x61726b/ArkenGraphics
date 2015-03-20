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
	m_Vector = DirectX::XMVectorZero();
}
//--------------------------------------------------------------------------------
ArkVectorParameter11::ArkVectorParameter11(ArkVectorParameter11& copy)
{
	m_Vector = copy.m_Vector;
}
//--------------------------------------------------------------------------------
ArkVectorParameter11::~ArkVectorParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkVectorParameter11::SetParameterData(void* pData,unsigned int thread )
{
	if(0 != memcpy(pData,&m_Vector,sizeof(DirectX::XMVECTOR)))
	{
		m_Vector = *reinterpret_cast<DirectX::XMVECTOR*>(pData);
	}
}
//--------------------------------------------------------------------------------
const ArkParamType ArkVectorParameter11::GetParameterType()
{
	return ArkParamType::VECTOR;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkVectorParameter11::GetVector()
{
	return m_Vector;
}
//--------------------------------------------------------------------------------
void ArkVectorParameter11::SetVector( DirectX::XMVECTOR v )
{
	m_Vector = v;
}
//--------------------------------------------------------------------------------
void* ArkVectorParameter11::operator new(size_t i)
{
	return _mm_malloc(i,16);
}
//--------------------------------------------------------------------------------
void ArkVectorParameter11::operator delete(void* p)
{
	_mm_free(p);
}
