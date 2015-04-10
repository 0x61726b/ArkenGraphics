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
	ArkShader11SPtr pShader = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11PixelShader* pS = 0;

	if( pShader )
	{
		pS = std::dynamic_pointer_cast< ArkPixelShader11 >( pShader )->m_pPixelShader;
	}
	pContext->PSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkPixelShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext,int count )
{
	pContext->PSSetConstantBuffers(
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkPixelShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext,int count )
{
	pContext->PSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkPixelShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext,int count )
{
		pContext->PSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(),
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}