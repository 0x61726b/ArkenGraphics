//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShader11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShader11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
ArkShader11::ArkShader11() :
	FileName(),
	Function(),
	ShaderModel(),
	ShaderText(),
	m_pCompiledShader( nullptr ),
	m_pReflection( nullptr )
{
}
//--------------------------------------------------------------------------------
ArkShader11::~ArkShader11()
{
	COM_Release( m_pCompiledShader );
	Safe_Delete( m_pReflection );
	ArkLog::Get( LogType::Renderer ).Output(L"Releasing Shader!");
}
//--------------------------------------------------------------------------------
void ArkShader11::SetReflection( ArkShaderReflection11* pReflection )
{
	m_pReflection = pReflection;
}
//--------------------------------------------------------------------------------
ArkShaderReflection11* ArkShader11::GetReflection( )
{
	return( m_pReflection );
}
//--------------------------------------------------------------------------------
std::wstring ArkShader11::ToString()
{
	std::wstringstream s;

	s << L"[" << FileName << L"][" << Function << L"][" << ShaderModel << L"]";

	return( s.str() );
}
//--------------------------------------------------------------------------------
