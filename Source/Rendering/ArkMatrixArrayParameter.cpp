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
ArkMatrixArrayParameter11::ArkMatrixArrayParameter11()
{
	DirectX::XMMATRIX i = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_Matrix,i);
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11::ArkMatrixArrayParameter11(ArkMatrixArrayParameter11& copy)
{
	m_Matrix = copy.m_Matrix;
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11::~ArkMatrixArrayParameter11()
{
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameter11::SetParameterData(void* pData,unsigned int thread)
{
	if(0 != memcpy(pData,&m_Matrix,sizeof(DirectX::XMFLOAT4X4)))
	{
		m_Matrix = *reinterpret_cast<DirectX::XMFLOAT4X4*>(pData);
	}
}
//--------------------------------------------------------------------------------
const ArkParamType ArkMatrixArrayParameter11::GetParameterType()
{
	return ArkParamType::MATRIX_ARRAY;
}
//--------------------------------------------------------------------------------
DirectX::XMFLOAT4X4 ArkMatrixArrayParameter11::GetMatrixArray()
{
	return m_Matrix;
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameter11::SetMatrixArray( DirectX::XMFLOAT4X4& v )
{
	m_Matrix = v;
}

