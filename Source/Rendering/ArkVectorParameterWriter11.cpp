//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkVectorParameterWriter11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkVectorParameterWriter11::ArkVectorParameterWriter11()
	: m_Value( DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ) )
{
}
//--------------------------------------------------------------------------------
ArkVectorParameterWriter11::~ArkVectorParameterWriter11()
{
}
//--------------------------------------------------------------------------------
void ArkVectorParameterWriter11::SetRenderParameterRef( std::shared_ptr<ArkVectorParameter11> pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ArkVectorParameterWriter11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetVectorParameter( m_pParameter, &m_Value );
}
//--------------------------------------------------------------------------------
void ArkVectorParameterWriter11::SetValue( const DirectX::XMVECTOR& Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkVectorParameterWriter11::GetValue()
{
	return( m_Value );
}
//--------------------------------------------------------------------------------
void ArkVectorParameterWriter11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkRenderParameter11> ArkVectorParameterWriter11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------
