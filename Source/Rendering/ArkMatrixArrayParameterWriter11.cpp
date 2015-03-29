//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixArrayParameterWriter11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMatrixArrayParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMatrixArrayParameterWriter11::ArkMatrixArrayParameterWriter11()
{
	m_pValue = 0;
	m_iCount = 0;
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameterWriter11::~ArkMatrixArrayParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkMatrixArrayParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetMatrixArrayParameter( m_pParameter, m_iCount, m_pValue );
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameterWriter11::SetValue( DirectX::XMMATRIX* Value )
{
	m_pValue = Value;
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameterWriter11::SetCount( int count )
{
	m_iCount = count;
}
//--------------------------------------------------------------------------------
void ArkMatrixArrayParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( m_pValue );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkMatrixArrayParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------
