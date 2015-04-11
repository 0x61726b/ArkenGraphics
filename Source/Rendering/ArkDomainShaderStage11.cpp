//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkDomainShaderStage11.h"
#include "ArkDomainShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkDomainShaderStage11::ArkDomainShaderStage11()
{

}
//--------------------------------------------------------------------------------
ArkDomainShaderStage11::~ArkDomainShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkDomainShaderStage11::GetType()
{
	return ShaderType::DOMAIN_SHADER;
}
//--------------------------------------------------------------------------------
void ArkDomainShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11SPtr pShader = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11DomainShader* pS = 0;

	if( pShader )
	{
		pS = std::dynamic_pointer_cast< ArkDomainShader11 >( pShader )->m_pDomainShader;
	}
	pContext->DSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkDomainShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext,int count )
{
	pContext->DSSetConstantBuffers(
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkDomainShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext,int count )
{
	pContext->DSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkDomainShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext,int count )
{
		pContext->DSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(),
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}
//--------------------------------------------------------------------------------
void ArkDomainShaderStage11::BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count )
{
	//Nope
}