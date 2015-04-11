//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkComputeShaderStage11.h
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkHullShaderStage11.h"
#include "ArkHullShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkHullShaderStage11::ArkHullShaderStage11()
{
}
//--------------------------------------------------------------------------------
ArkHullShaderStage11::~ArkHullShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkHullShaderStage11::GetType()
{
	return( HULL_SHADER );
}
//--------------------------------------------------------------------------------
void ArkHullShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11SPtr pShaderDX11 = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11HullShader* pShader = 0;

	if ( pShaderDX11 ) {
		pShader = std::dynamic_pointer_cast< ArkHullShader11 >( pShaderDX11 )->m_pHullShader;
	}

	pContext->HSSetShader( pShader, 0, 0 );
}
//--------------------------------------------------------------------------------
void ArkHullShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext, int count )
{
	pContext->HSSetConstantBuffers( 
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkHullShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext, int count )
{
	pContext->HSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkHullShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext, int count )
{
	pContext->HSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(),
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}
//--------------------------------------------------------------------------------
void ArkHullShaderStage11::BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count )
{
	//Nope
}
//--------------------------------------------------------------------------------


