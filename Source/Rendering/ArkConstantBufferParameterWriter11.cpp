//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameterWriter11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkConstantBufferParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkConstantBufferParameterWriter11::ArkConstantBufferParameterWriter11()
{
}
//--------------------------------------------------------------------------------
ArkConstantBufferParameterWriter11::~ArkConstantBufferParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkConstantBufferParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkConstantBufferParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkConstantBufferParameterWriter11::SetValue( ResourcePtr Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
void ArkConstantBufferParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetConstantBufferParameter( m_pParameter, m_Value );
}
//--------------------------------------------------------------------------------
void ArkConstantBufferParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value->m_iResource );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkConstantBufferParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------

