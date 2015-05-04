//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkRenderEffect11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderEffect11.h"
#include "PipelineManager.h"
#include "IParameterManager.h"
#include "ArkConstantBuffer11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkRenderEffect11::ArkRenderEffect11()
{
	for(int i = 0; i < 6; i++) {
		m_aiIndices[i] = -1;
		m_apShaders[i] = nullptr;
	}

	m_iBlendState = -1;
	m_iDepthStencilState = -1;
	m_iRasterizerState = -1;
	m_uStencilRef = 0;
}
//--------------------------------------------------------------------------------
ArkRenderEffect11::~ArkRenderEffect11()
{
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetVertexShader(int index)
{
	m_aiIndices[VERTEX_SHADER] = index;
	m_apShaders[VERTEX_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[VERTEX_SHADER] != nullptr) {
		if(m_apShaders[VERTEX_SHADER]->GetType() != VERTEX_SHADER) {
			ArkLog::Get(LogType::Renderer).Output(L"Trying to bind a non-vertex shader to the vertex shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetHullShader(int index)
{
	m_aiIndices[HULL_SHADER] = index;
	m_apShaders[HULL_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[HULL_SHADER] != nullptr) {
		if(m_apShaders[HULL_SHADER]->GetType() != HULL_SHADER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to bind a non-HULL_SHADER shader to the HULL_SHADER shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetDomainShader(int index)
{
	m_aiIndices[DOMAIN_SHADER] = index;
	m_apShaders[DOMAIN_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[DOMAIN_SHADER] != nullptr) {
		if(m_apShaders[DOMAIN_SHADER]->GetType() != DOMAIN_SHADER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to bind a non-DOMAIN_SHADER shader to the DOMAIN_SHADER shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetGeometryShader(int index)
{
	m_aiIndices[GEOMETRY_SHADER] = index;
	m_apShaders[GEOMETRY_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[GEOMETRY_SHADER] != nullptr) {
		if(m_apShaders[GEOMETRY_SHADER]->GetType() != GEOMETRY_SHADER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to bind a non-GEOMETRY_SHADER shader to the GEOMETRY_SHADER shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetPixelShader(int index)
{
	m_aiIndices[PIXEL_SHADER] = index;
	m_apShaders[PIXEL_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[PIXEL_SHADER] != nullptr) {
		if(m_apShaders[PIXEL_SHADER]->GetType() != PIXEL_SHADER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to bind a non-pixel shader to the pixel shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::SetComputeShader(int index)
{
	m_aiIndices[COMPUTE_SHADER] = index;
	m_apShaders[COMPUTE_SHADER] = ArkRenderer11::Get()->GetShader(index).get();

	if(m_apShaders[COMPUTE_SHADER] != nullptr) {
		if(m_apShaders[COMPUTE_SHADER]->GetType() != COMPUTE_SHADER) {
			ArkLog::Get(LogType::Renderer).Write(L"Trying to bind a non-COMPUTE_SHADER shader to the COMPUTE_SHADER shader...");
		}
	}

	UpdateConstantBufferList();
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::UpdateConstantBufferList()
{
	// Empty the list to start out.

	m_uniqueConstBuffers.clear();


	// Check each stage to see if it has a valid shader program.  If so, then 
	// inspect its reflection data to ensure that the overall list of constant
	// buffers includes that stage's needs.

	for(int stage = 0; stage < 6; stage++) {

		ArkShader11* pShader = m_apShaders[stage];

		if(nullptr != pShader)
		{

			ArkShaderReflection11* pReflection = pShader->GetReflection();

			if(pReflection != nullptr)
			{

				for(unsigned int i = 0; i < pReflection->ConstantBuffers.size(); i++)
				{

					std::shared_ptr<ArkRenderParameter11> pParameter = pReflection->ConstantBuffers[i].pParamRef;

					// If it isn't already included, then add it to the list.

					bool bAlreadyThere = false;
					for(auto pExistingParam : m_uniqueConstBuffers) {
						if(pParameter == pExistingParam) {
							bAlreadyThere = true;
							break;
						}
					}

					if(!bAlreadyThere) {
						m_uniqueConstBuffers.push_back(pParameter);
					}
					else
					{
						ArkLog::Get(LogType::Renderer).Write(L"Skipped adding a duplicate constant buffer...");
					}

				}
			}
		}
	}
}
//--------------------------------------------------------------------------------
void ArkRenderEffect11::ConfigurePipeline(PipelineManager* pPipeline,IParameterManager* pParamManager)
{
	if(m_iBlendState != -1) {
		pPipeline->OutputMergerStage.CurrentState.BlendState.SetState(m_iBlendState);
	}
	else {
		pPipeline->OutputMergerStage.CurrentState.BlendState.SetState(0);
	}

	if(m_iDepthStencilState != -1) {
		pPipeline->OutputMergerStage.CurrentState.DepthStencilState.SetState(m_iDepthStencilState);
		pPipeline->OutputMergerStage.CurrentState.StencilRef.SetState(m_uStencilRef);
	}
	else {
		pPipeline->OutputMergerStage.CurrentState.DepthStencilState.SetState(0);
	}

	if(m_iRasterizerState != -1) {
		pPipeline->RasterizerStage.CurrentState.RasterizerState.SetState(m_iRasterizerState);
	}
	else {
		pPipeline->RasterizerStage.CurrentState.RasterizerState.SetState(0);
	}

	for(auto pParameter : m_uniqueConstBuffers) 
	{
		std::shared_ptr<ArkConstantBuffer11> cbuffer = ArkRenderer11::Get()->GetConstantBufferByIndex(pParamManager->GetConstantBufferParameter(pParameter));
		cbuffer->EvaluateMappings(pPipeline,pParamManager);
	}

	pPipeline->BindShader( VERTEX_SHADER, m_aiIndices[VERTEX_SHADER], pParamManager );
	pPipeline->BindShader( HULL_SHADER, m_aiIndices[HULL_SHADER], pParamManager );
	pPipeline->BindShader( DOMAIN_SHADER, m_aiIndices[DOMAIN_SHADER], pParamManager );
	pPipeline->BindShader( GEOMETRY_SHADER, m_aiIndices[GEOMETRY_SHADER], pParamManager );
	pPipeline->BindShader( PIXEL_SHADER, m_aiIndices[PIXEL_SHADER], pParamManager );
	pPipeline->BindShader( COMPUTE_SHADER, m_aiIndices[COMPUTE_SHADER], pParamManager );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetVertexShader( )
{
	return( m_aiIndices[VERTEX_SHADER] );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetHullShader( )
{
	return( m_aiIndices[HULL_SHADER] );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetDomainShader( )
{
	return( m_aiIndices[DOMAIN_SHADER] );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetGeometryShader( )
{
	return( m_aiIndices[GEOMETRY_SHADER] );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetPixelShader( )
{
	return( m_aiIndices[PIXEL_SHADER] );
}
//--------------------------------------------------------------------------------
int ArkRenderEffect11::GetComputeShader( )
{
	return( m_aiIndices[COMPUTE_SHADER] );
}
//--------------------------------------------------------------------------------