//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkVertexShaderStage11.h"
#include "ArkVertexShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkVertexShaderStage11::ArkVertexShaderStage11()
{

}
//--------------------------------------------------------------------------------
ArkVertexShaderStage11::~ArkVertexShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkVertexShaderStage11::GetType()
{
	return ShaderType::VERTEX_SHADER;
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11* pShader = pRenderer->GetShader( State.ShaderProgram );

	ID3D11VertexShader* pS = 0;

	if( pShader )
	{
		pS = reinterpret_cast< ArkVertexShader11* >( pShader )->m_pVertexShader;
	}
	pContext->VSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext )
{
	pContext->VSSetConstantBuffers(0,
		State.ConstantBuffers.size(),
		&State.ConstantBuffers[0]
		);
}