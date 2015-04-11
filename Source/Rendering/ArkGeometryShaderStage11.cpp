//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkVertexShaderStage.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkGeometryShaderStage11.h"
#include "ArkGeometryShader11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkGeometryShaderStage11::ArkGeometryShaderStage11()
{
}
//--------------------------------------------------------------------------------
ArkGeometryShaderStage11::~ArkGeometryShaderStage11()
{
}
//--------------------------------------------------------------------------------
ShaderType ArkGeometryShaderStage11::GetType()
{
	return( GEOMETRY_SHADER );
}
//--------------------------------------------------------------------------------
void ArkGeometryShaderStage11::BindShaderProgram( ID3D11DeviceContext* pContext )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11SPtr pShader = pRenderer->GetShader( CurrentState.ShaderProgram.GetState() );

	ID3D11GeometryShader* pS = 0;

	if( pShader )
	{
		pS = std::dynamic_pointer_cast< ArkGeometryShader11 >( pShader )->m_pGeometryShader;
	}
	pContext->GSSetShader( pS,0,0 );
}
//--------------------------------------------------------------------------------
void ArkGeometryShaderStage11::BindConstantBuffers( ID3D11DeviceContext* pContext, int count )
{
	pContext->GSSetConstantBuffers( 
		CurrentState.ConstantBuffers.GetStartSlot(),
		CurrentState.ConstantBuffers.GetRange(),
		CurrentState.ConstantBuffers.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkGeometryShaderStage11::BindSamplerStates( ID3D11DeviceContext* pContext, int count )
{
	pContext->GSSetSamplers( 
		CurrentState.SamplerStates.GetStartSlot(),
		CurrentState.SamplerStates.GetRange(),
		CurrentState.SamplerStates.GetFirstSlotLocation() );
}
//--------------------------------------------------------------------------------
void ArkGeometryShaderStage11::BindShaderResourceViews( ID3D11DeviceContext* pContext, int count )
{
	pContext->GSSetShaderResources( 
		CurrentState.ShaderResourceViews.GetStartSlot(),
		CurrentState.ShaderResourceViews.GetRange(), 
		CurrentState.ShaderResourceViews.GetFirstSlotLocation() ); 
}
//--------------------------------------------------------------------------------
void ArkGeometryShaderStage11::BindUnorderedAccessViews( ID3D11DeviceContext* pContext, int count )
{
	// Do nothing - the geometry shader doesn't support UAV's!
}
//--------------------------------------------------------------------------------



