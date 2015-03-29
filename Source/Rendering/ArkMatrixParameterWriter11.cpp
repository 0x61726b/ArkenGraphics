//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameterWriter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMatrixParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMatrixParameterWriter11::ArkMatrixParameterWriter11()
{
}
//--------------------------------------------------------------------------------
ArkMatrixParameterWriter11::~ArkMatrixParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkMatrixParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkMatrixParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkMatrixParameterWriter11::SetValue( const DirectX::XMMATRIX& Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
void ArkMatrixParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetMatrixParameter( m_pParameter, &m_Value );
}
//--------------------------------------------------------------------------------
void ArkMatrixParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkMatrixParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------

