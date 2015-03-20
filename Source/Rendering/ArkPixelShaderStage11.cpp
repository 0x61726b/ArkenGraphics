//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkPixelShaderStage11.h"
#include "ArkPixelShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkPixelShaderStage11::ArkPixelShaderStage11()
{

}
//--------------------------------------------------------------------------------
ArkPixelShaderStage11::~ArkPixelShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkPixelShaderStage11::GetType()
{
	return ShaderType::PIXEL_SHADER;
}
//--------------------------------------------------------------------------------
void ArkPixelShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11* pShader = pRenderer->GetShader( State.ShaderProgram );

	ID3D11PixelShader* pS = 0;

	if( pShader )
	{
		pS = reinterpret_cast< ArkPixelShader11* >( pShader )->m_pPixelShader;
	}
	pContext->PSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkPixelShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext )
{
	if( State.ConstantBuffers.size() > 0)
		pContext->PSSetConstantBuffers(0,
		State.ConstantBuffers.size(),
		&State.ConstantBuffers[0]
		);
}