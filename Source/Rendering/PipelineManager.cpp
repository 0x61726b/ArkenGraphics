//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RendererDX11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkLog.h"
#include "PipelineManager.h"
#include "IParameterManager.h"

#include "ArkRenderParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkConstantBufferParameter11.h"
#include "ArkVectorParameter11.h"

#include "ArkVertexShader11.h"
#include "ArkPixelShader11.h"

#include "Dx11Resource.h"
#include "Dx11RenderTargetView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
PipelineManager::PipelineManager()
{
	ShaderStages[VERTEX_SHADER] = &VertexShaderStage;
	ShaderStages[PIXEL_SHADER]  = &PixelShaderStage;
}
//--------------------------------------------------------------------------------
PipelineManager::~PipelineManager()
{
	if( m_pContext ) m_pContext->ClearState();
	if( m_pContext ) m_pContext->Flush();
}
//--------------------------------------------------------------------------------
void PipelineManager::SetDeviceContext( DeviceContextComPtr Context,D3D_FEATURE_LEVEL FeatureLevel )
{
	m_pContext = Context;
	m_FeatureLevel = FeatureLevel;

	OutputMergerStage.SetFeatureLevel( FeatureLevel );
	RasterizerStage.SetFeatureLevel( FeatureLevel );
	InputAssemblerStage.SetFeatureLevel( FeatureLevel );
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyRenderTargets()
{
	OutputMergerStage.ApplyRenderTargets( m_pContext.Get() );
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearRenderTargets()
{
	OutputMergerStage.ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineState()
{
	VertexShaderStage.ClearState();
	PixelShaderStage.ClearState();

	OutputMergerStage.ClearState();
	RasterizerStage.ClearState();
	InputAssemblerStage.ClearState();

	m_pContext->ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyPipelineResources()
{
	VertexShaderStage.ApplyState( m_pContext.Get () );
	PixelShaderStage.ApplyState( m_pContext.Get() );

	RasterizerStage.ApplyState( m_pContext.Get() );
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineResources()
{
	VertexShaderStage.ClearState();
	PixelShaderStage.ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyInputResources()
{
	InputAssemblerStage.ApplyState( m_pContext.Get() );
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearInputResources()
{
	InputAssemblerStage.ClearState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearBuffers(float color[],float depth)
{
	ID3D11RenderTargetView* pRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { NULL };
	UINT viewCount = OutputMergerStage.GetCurrentState().GetRenderTargetCount();

	for( UINT i = 0; i < viewCount; ++i )
	{
		int rtv = OutputMergerStage.State.GetRenderTargetView(i);
		Dx11RenderTargetView& rtView = ArkRenderer11::Get()->GetRenderTargetViewByIndex(rtv);
		pRenderTargetViews[i] = rtView.m_pRenderTargetView.Get();

		if( pRenderTargetViews[i] != nullptr )
		{
			m_pContext->ClearRenderTargetView(pRenderTargetViews[i],color);
		}
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindConstantBufferParameter( ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam, UINT slot,IParameterManager* pParamManager )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	unsigned int tID = pParamManager->GetID();;
	if( pParam != 0 )
	{
		if( pParam->GetParameterType() == CBUFFER )
		{
			std::shared_ptr<ArkConstantBufferParameter11> pBuffer = std::dynamic_pointer_cast< ArkConstantBufferParameter11 >( pParam );
			int ID = pBuffer->GetIndex(tID);

			std::shared_ptr<Dx11Resource> pResource = pRenderer->GetResourceByIndex( ID );
			if( pResource || ( ID == -1 ))
			{
				ID3D11Buffer* pBuffer = 0;

				if( ID >= 0 )
				{
					pBuffer = (ID3D11Buffer*)pResource->GetResource();
				}
				ShaderStages[type]->State.ConstantBuffers.push_back(pBuffer);
			}
			else
			{
				ArkLog::Get( LogType::Renderer ).Output(L"Invalid constant buffer ID!");
			}
		}
		else
		{
			ArkLog::Get( LogType::Renderer ).Output(L"Non-constant buffer tried to set ID!");
		}
	}
	else
	{
		ArkLog::Get( LogType::Renderer ).Output(L"Tried to set a non-existing parameter!");
	}	
}
//--------------------------------------------------------------------------------
void PipelineManager::BindShader( ShaderType type,int ID,IParameterManager* pParamManager )
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11* pShader11 = pRenderer->GetShader( ID );

	ShaderStages[type]->State.ShaderProgram = ID;
	if( pShader11 )
	{
		if( pShader11->GetType() == type )
		{
			pShader11->GetReflection()->BindParameters(type,this,pParamManager);
		}
		else
		{
			ArkLog::Get( LogType::Renderer ).Output(L"Tried to set wrong type of shader ID!");
		}
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::Draw( ArkRenderEffect11& effect, ResourcePtr vb, ResourcePtr ib,
					int inputLayout, D3D11_PRIMITIVE_TOPOLOGY primType,
					UINT vertexStride, UINT numIndices, IParameterManager* pParamManager)
{
	InputAssemblerStage.ClearState();

	InputAssemblerStage.CurrentState.SetPrimitiveTopology( primType );

	if( vb != NULL )
	{
		InputAssemblerStage.CurrentState.AddVertexBuffer( vb->m_iResource );
		InputAssemblerStage.CurrentState.AddVertexBufferStride( vertexStride );
		InputAssemblerStage.CurrentState.AddVertexBufferOffsets(0);
	}
	if( ib != NULL )
	{
		InputAssemblerStage.CurrentState.SetIndexBuffer( ib->m_iResource );
		InputAssemblerStage.CurrentState.SetIndexBufferFormat( DXGI_FORMAT_R32_UINT );
	}

	if( vb != NULL )
	{
		InputAssemblerStage.CurrentState.SetInputLayout( inputLayout );
	}

	InputAssemblerStage.ApplyState( m_pContext.Get() );

	ClearPipelineResources();
	effect.ConfigurePipeline(this,pParamManager);
	ApplyPipelineResources();

	m_pContext->DrawIndexed( numIndices,0,0 );
}
//--------------------------------------------------------------------------------
D3D11_MAPPED_SUBRESOURCE PipelineManager::MapResource( Dx11Resource* pArkResource, UINT subresource, D3D11_MAP actions, UINT flags )
{
	D3D11_MAPPED_SUBRESOURCE Data;
	Data.pData = NULL;
	Data.DepthPitch = Data.RowPitch = 0;

	if ( nullptr == pArkResource ) {
		ArkLog::Get(LogType::Renderer).Output( L"Trying to map a subresource that doesn't exist!!!" );
		return( Data );
	}
	// TODO: Update this to use a ComPtr!
	// Acquire the native resource pointer.
	ID3D11Resource* pResource = 0;
	pResource = pArkResource->GetResource();

	if ( nullptr == pResource ) {
		ArkLog::Get(LogType::Renderer).Output( L"Trying to map a subresource that has no native resource in it!!!" );
		return( Data );
	}

	// Perform the mapping of the resource.
	HRESULT hr = m_pContext->Map( pResource, subresource, actions, flags, &Data );
	
	if ( FAILED( hr ) ) {
		ArkLog::Get(LogType::Renderer).Output( L"Failed to map resource!" );
	}

	return( Data );
}
//--------------------------------------------------------------------------------
void PipelineManager::UnMapResource(Dx11Resource* pArkResource,UINT subresource)
{
	if ( NULL == pArkResource ) {
		ArkLog::Get(LogType::Renderer).Output( L"Trying to unmap a subresource that doesn't exist!!!" );
		return;
	}

	// Acquire the native resource pointer.
	ID3D11Resource* pResource = 0;
	pResource = pArkResource->GetResource();

	if ( NULL == pResource ) {
		ArkLog::Get(LogType::Renderer).Output( L"Trying to unmap a subresource that has no native resource in it!!!" );
		return;
	}

	// Unmap the resource - there is no HRESULT returned, so trust that it works...
	m_pContext->Unmap( pResource, subresource );
}