//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkComputeShaderStage11.h"
#include "ArkComputeShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkComputeShaderStage11::ArkComputeShaderStage11()
{

}
//--------------------------------------------------------------------------------
ArkComputeShaderStage11::~ArkComputeShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkComputeShaderStage11::GetType()
{
	return ShaderType::COMPUTE_SHADER;
}
//--------------------------------------------------------------------------------
void ArkComputeShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11SPtr pShader = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11ComputeShader* pS = 0;

	if( pShader )
	{
		pS = std::dynamic_pointer_cast< ArkComputeShader11 >( pShader )->m_pComputeShader;
	}
	pContext->CSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkComputeShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext,int count )
{
	pContext->CSSetConstantBuffers(
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkComputeShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext,int count )
{
	pContext->CSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkComputeShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext,int count )
{
		pContext->CSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(),
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}
//--------------------------------------------------------------------------------
void ArkComputeShaderStage11::BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count )
{
	unsigned int minStartSlot = 
		min( CurrentState.UnorderedAccessViews.GetStartSlot(),
		CurrentState.UAVInitialCounts.GetStartSlot() );

	unsigned int maxEndSlot =
		max( CurrentState.UnorderedAccessViews.GetEndSlot(),
		CurrentState.UAVInitialCounts.GetEndSlot() );

	pContext->CSSetUnorderedAccessViews( 
		minStartSlot,
		maxEndSlot - minStartSlot + 1,
		CurrentState.UnorderedAccessViews.GetSlotLocation( minStartSlot ),
		CurrentState.UAVInitialCounts.GetSlotLocation( minStartSlot ) );
}