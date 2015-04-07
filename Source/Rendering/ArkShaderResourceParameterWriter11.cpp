//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderResourceParameterWriter11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShaderResourceParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkShaderResourceParameterWriter11::ArkShaderResourceParameterWriter11()
{
}
//--------------------------------------------------------------------------------
ArkShaderResourceParameterWriter11::~ArkShaderResourceParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkShaderResourceParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkShaderResourceParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkShaderResourceParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetShaderResourceParameter( m_pParameter, m_Value );
}
//--------------------------------------------------------------------------------
void ArkShaderResourceParameterWriter11::SetValue( ResourcePtr Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
void ArkShaderResourceParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value->m_iResourceSRV );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkShaderResourceParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------
