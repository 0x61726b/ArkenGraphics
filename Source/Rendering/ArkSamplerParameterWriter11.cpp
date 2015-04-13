//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryGenerator11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSamplerParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkSamplerParameterWriter11::ArkSamplerParameterWriter11()
{
	m_Value = -1;
}
//--------------------------------------------------------------------------------
ArkSamplerParameterWriter11::~ArkSamplerParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkSamplerParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkSamplerParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkSamplerParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetSamplerParameter( m_pParameter, &m_Value );
}
//--------------------------------------------------------------------------------
void ArkSamplerParameterWriter11::SetValue( int Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
void ArkSamplerParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkSamplerParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------
