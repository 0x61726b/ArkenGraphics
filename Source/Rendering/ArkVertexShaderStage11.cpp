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
	ArkShader11SPtr pShader = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11VertexShader* pS = 0;

	if( pShader )
	{
		pS = std::dynamic_pointer_cast< ArkVertexShader11 >( pShader )->m_pVertexShader;
	}
	pContext->VSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext,int count )
{
	pContext->VSSetConstantBuffers( 
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext,int count )
{
	pContext->VSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext,int count )
{
		pContext->VSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(),
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}
//--------------------------------------------------------------------------------
void ArkVertexShaderStage11::BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count )
{

}