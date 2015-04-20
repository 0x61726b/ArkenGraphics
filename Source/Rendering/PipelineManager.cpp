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
#include "Dx11ViewPort.h"
#include "IParameterManager.h"

#include "ArkRenderParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkConstantBufferParameter11.h"
#include "ArkVectorParameter11.h"
#include "ArkUnorderedAccessParameter11.h"

#include "ArkVertexShader11.h"
#include "ArkPixelShader11.h"
#include "ArkComputeShader11.h"
#include "ArkHullShader11.h"
#include "ArkDomainShader11.h"
#include "ArkGeometryShader11.h"

#include "Dx11Resource.h"
#include "Dx11RenderTargetView.h"
#include "Dx11DepthStencilView.h"
#include "Dx11UnorderedAccessView.h"

#include "ArkSamplerParameter11.h"
#include "Dx11ShaderResourceView.h"

#include "ArkCommandList11.h"


#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )

//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
PipelineManager::PipelineManager()
{
	m_iCurrentQuery = 0;

	ZeroMemory(&m_PipelineStatsData,sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS));

	ShaderStages[VERTEX_SHADER] = &VertexShaderStage;
	ShaderStages[PIXEL_SHADER]  = &PixelShaderStage;
	ShaderStages[COMPUTE_SHADER] = &ComputeShaderStage;
	ShaderStages[GEOMETRY_SHADER] = &GeometryShaderStage;
	ShaderStages[HULL_SHADER] = &HullShaderStage;
	ShaderStages[DOMAIN_SHADER] = &DomainShaderStage;

}
//--------------------------------------------------------------------------------
PipelineManager::~PipelineManager()
{
	if(m_pContext) m_pContext->ClearState();
	if(m_pContext) m_pContext->Flush();
}
//--------------------------------------------------------------------------------
void PipelineManager::SetDeviceContext(DeviceContextComPtr Context,D3D_FEATURE_LEVEL FeatureLevel)
{
	m_pContext = Context;
	m_FeatureLevel = FeatureLevel;

	m_pAnnotation = nullptr;
	HRESULT hr = m_pContext.CopyTo(m_pAnnotation.GetAddressOf());

	ShaderStages[VERTEX_SHADER]->SetFeatureLevel(FeatureLevel);
	ShaderStages[PIXEL_SHADER]->SetFeatureLevel(FeatureLevel);
	ShaderStages[COMPUTE_SHADER]->SetFeatureLevel(FeatureLevel);
	ShaderStages[GEOMETRY_SHADER]->SetFeatureLevel(FeatureLevel);
	ShaderStages[HULL_SHADER]->SetFeatureLevel(FeatureLevel);
	ShaderStages[DOMAIN_SHADER]->SetFeatureLevel(FeatureLevel);


	OutputMergerStage.SetFeatureLevel(FeatureLevel);
	RasterizerStage.SetFeatureLevel(FeatureLevel);
	InputAssemblerStage.SetFeatureLevel(FeatureLevel);
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyRenderTargets()
{
	OutputMergerStage.ApplyRenderTargets(m_pContext.Get());
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearRenderTargets()
{
	OutputMergerStage.ClearCurrentState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineState()
{
	InputAssemblerStage.ClearPreviousState();
	InputAssemblerStage.ClearCurrentState();


	VertexShaderStage.ClearPreviousState();
	VertexShaderStage.ClearCurrentState();

	HullShaderStage.ClearPreviousState();
	HullShaderStage.ClearCurrentState();

	DomainShaderStage.ClearPreviousState();
	DomainShaderStage.ClearCurrentState();

	GeometryShaderStage.ClearPreviousState();
	GeometryShaderStage.ClearCurrentState();

	StreamOutputStage.ClearPreviousState();
	StreamOutputStage.ClearCurrentState();

	RasterizerStage.ClearPreviousState();
	RasterizerStage.ClearCurrentState();

	PixelShaderStage.ClearPreviousState();
	PixelShaderStage.ClearCurrentState();

	OutputMergerStage.ClearCurrentState();
	OutputMergerStage.ClearPreviousState();

	ComputeShaderStage.ClearPreviousState();
	ComputeShaderStage.ClearCurrentState();



	m_pContext->ClearState();

	if(m_pContext->GetType() == D3D11_DEVICE_CONTEXT_DEFERRED) {
		ID3D11CommandList* pList;
		m_pContext->FinishCommandList(true,&pList);
		pList->Release();
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineSRVs()
{
	VertexShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
	HullShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
	DomainShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
	GeometryShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
	PixelShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
	ComputeShaderStage.CurrentState.ShaderResourceViews.InitializeStates();
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyPipelineResources()
{
	VertexShaderStage.ApplyCurrentState(m_pContext.Get());
	HullShaderStage.ApplyCurrentState(m_pContext.Get());
	DomainShaderStage.ApplyCurrentState(m_pContext.Get());
	GeometryShaderStage.ApplyCurrentState(m_pContext.Get());
	PixelShaderStage.ApplyCurrentState(m_pContext.Get());
	ComputeShaderStage.ApplyCurrentState(m_pContext.Get());

	StreamOutputStage.ApplyDesiredState(m_pContext.Get());
	RasterizerStage.ApplyCurrentState(m_pContext.Get());
	OutputMergerStage.ApplyDepthStencilStatesAndBlendStates(m_pContext.Get());
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearPipelineResources()
{
	VertexShaderStage.ClearCurrentState();
	HullShaderStage.ClearCurrentState();
	DomainShaderStage.ClearCurrentState();
	GeometryShaderStage.ClearCurrentState();
	PixelShaderStage.ClearCurrentState();
	ComputeShaderStage.ClearCurrentState();

	StreamOutputStage.ClearCurrentState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ApplyInputResources()
{
	InputAssemblerStage.ApplyCurrentState(m_pContext.Get());
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearInputResources()
{
	InputAssemblerStage.ClearCurrentState();
}
//--------------------------------------------------------------------------------
void PipelineManager::ClearBuffers(float color[],float depth,UINT stencil)
{
	ID3D11RenderTargetView* pRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] ={NULL};
	ID3D11DepthStencilView* pDepthStencilView = 0;


	UINT viewCount = OutputMergerStage.GetPreviousState().GetRenderTargetCount();

	for(UINT i = 0; i < viewCount; ++i)
	{
		int rtv = OutputMergerStage.CurrentState.RenderTargetViews.GetState(i);
		Dx11RenderTargetView& rtView = ArkRenderer11::Get()->GetRenderTargetViewByIndex(rtv);
		pRenderTargetViews[i] = rtView.m_pRenderTargetView.Get();

		if(pRenderTargetViews[i] != nullptr)
		{
			m_pContext->ClearRenderTargetView(pRenderTargetViews[i],color);
		}
	}

	if(OutputMergerStage.CurrentState.DepthTargetViews.GetState() != -1)
	{
		int dsv = OutputMergerStage.CurrentState.DepthTargetViews.GetState();
		Dx11DepthStencilView DSV = ArkRenderer11::Get()->GetDepthStencilViewByIndex(dsv);
		pDepthStencilView = DSV.m_pDepthStencilView.Get();

		if(pDepthStencilView != nullptr)
			m_pContext->ClearDepthStencilView(pDepthStencilView,D3D11_CLEAR_DEPTH,depth,stencil);
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindConstantBufferParameter(ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam,UINT slot,IParameterManager* pParamManager)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	unsigned int tID = pParamManager->GetID();;
	if(pParam != 0)
	{
		if(pParam->GetParameterType() == CBUFFER)
		{
			std::shared_ptr<ArkConstantBufferParameter11> pBuffer = std::dynamic_pointer_cast<ArkConstantBufferParameter11>(pParam);
			int ID = pBuffer->GetIndex(tID);

			std::shared_ptr<Dx11Resource> pResource = pRenderer->GetResourceByIndex(ID);
			if(pResource || (ID == -1))
			{
				ID3D11Buffer* pBuffer = 0;

				if(ID >= 0)
				{
					pBuffer = (ID3D11Buffer*)pResource->GetResource();
				}
				ShaderStages[type]->CurrentState.ConstantBuffers.SetState(slot,pBuffer);
			}
			else
			{
				ArkLog::Get(LogType::Renderer).Output(L"Invalid constant buffer ID!");
			}
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Output(L"Non-constant buffer tried to set ID!");
		}
	}
	else
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tried to set a non-existing parameter!");
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindSamplerStateParameter(ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam,UINT slot,IParameterManager* pParamManager)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	unsigned int tID = pParamManager->GetID();

	if(pParam != 0) {

		// Check the type of the parameter
		if(pParam->GetParameterType() == SAMPLER) {

			std::shared_ptr<ArkSamplerParameter11> pResource =
				std::dynamic_pointer_cast<ArkSamplerParameter11>(pParam);

			int ID = pResource->GetIndex(tID);

			// Get the resource to be set, and pass it in to the desired shader type

			ID3D11SamplerState* pSampler = nullptr;

			if(ID >= 0) {
				SamplerStateComPtr pState = pRenderer->GetSamplerState(ID);
				pSampler = pState.Get();
			}

			ShaderStages[type]->CurrentState.SamplerStates.SetState(slot,pSampler);

		}
		else {
			ArkLog::Get(LogType::Renderer).Output(L"Tried to set a non-sampler state ID as a sampler state!");
		}
	}
	else {
		ArkLog::Get(LogType::Renderer).Output(L"Tried to set a non-existing parameter as a sampler state!");
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindShaderResourceParameter(ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam,UINT slot,IParameterManager* pParamManager)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	unsigned int tID = pParamManager->GetID();

	if(pParam != 0) {

		// Check the type of the parameter
		if(pParam->GetParameterType() == SHADER_RESOURCE) {
			std::shared_ptr<ArkShaderResourceParameter11> pResource =
				std::dynamic_pointer_cast<ArkShaderResourceParameter11>(pParam);

			int ID = pResource->GetIndex(tID);

			Dx11ShaderResourceView& view = pRenderer->GetShaderResourceViewByIndex(ID);
			ShaderStages[type]->CurrentState.ShaderResourceViews.SetState(slot,view.m_pShaderResourceView.Get());

		}
		else {
			ArkLog::Get(LogType::Renderer).Output(L"Tried to set a non-shader resource ID as a shader resource!");
		}
	}
	else {
		ArkLog::Get(LogType::Renderer).Output(L"Tried to set a non-existing parameter as a shader resource!");
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindUnorderedAccessParameter(ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam,UINT slot,IParameterManager* pParamManager)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();

	unsigned int tID = pParamManager->GetID();

	if ( pParam != 0 ) {

		// Check the type of the parameter
		if ( pParam->GetParameterType() == UOA ) {

			std::shared_ptr<ArkUnorderedAccessParameter11> pResource = 
				std::dynamic_pointer_cast<ArkUnorderedAccessParameter11>( pParam );

			int ID = pResource->GetIndex( tID ); 
			unsigned int initial = pResource->GetInitialCount( tID );

			Dx11UnorderedAccessView& view = pRenderer->GetUnorderedAccessViewByIndex( ID );

			ShaderStages[type]->CurrentState.UnorderedAccessViews.SetState( slot, view.m_pUnorderedAccessView.Get() );
			ShaderStages[type]->CurrentState.UAVInitialCounts.SetState( slot, initial );

		} else {
			ArkLog::Get(LogType::Renderer).Output( L"Tried to set a non-unordered access view ID as a unordered access view!" );
		}
	} else {
		ArkLog::Get(LogType::Renderer).Output( L"Tried to set a non-existing parameter as a unordered access view!" );
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::BindShader(ShaderType type,int ID,IParameterManager* pParamManager)
{
	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	ArkShader11SPtr pShader11 = pRenderer->GetShader(ID);

	ShaderStages[type]->CurrentState.ShaderProgram.SetState(ID);
	if(pShader11)
	{
		if(pShader11->GetType() == type)
		{
			pShader11->GetReflection()->BindParameters(type,this,pParamManager);
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Output(L"Tried to set wrong type of shader ID!");
		}
	}
}
//--------------------------------------------------------------------------------
D3D11_MAPPED_SUBRESOURCE PipelineManager::MapResource(int rid,UINT subresource,D3D11_MAP actions,UINT flags)
{
	// Acquire the engine's resource wrapper.
	std::shared_ptr<Dx11Resource> pGlyphResource = 0;
	pGlyphResource = ArkRenderer11::Get()->GetResourceByIndex(rid);

	return(MapResource(pGlyphResource.get(),subresource,actions,flags));
}
//--------------------------------------------------------------------------------
void PipelineManager::UnMapResource(int rid,UINT subresource)
{
	// Acquire the engine's resource wrapper.
	std::shared_ptr<Dx11Resource>  pGlyphResource = 0;
	pGlyphResource = ArkRenderer11::Get()->GetResourceByIndex(rid);

	UnMapResource(pGlyphResource.get(),subresource);
}
//--------------------------------------------------------------------------------
D3D11_MAPPED_SUBRESOURCE PipelineManager::MapResource(ResourcePtr pResource,UINT subresource,D3D11_MAP actions,UINT flags)
{
	// Acquire the engine's resource wrapper.
	std::shared_ptr<Dx11Resource>  pGlyphResource = 0;
	pGlyphResource = ArkRenderer11::Get()->GetResourceByIndex(pResource->m_iResource);

	return(MapResource(pGlyphResource.get(),subresource,actions,flags));
}
//--------------------------------------------------------------------------------
void PipelineManager::UnMapResource(ResourcePtr pResource,UINT subresource)
{
	// Acquire the engine's resource wrapper.
	std::shared_ptr<Dx11Resource> pGlyphResource = 0;
	pGlyphResource = ArkRenderer11::Get()->GetResourceByIndex(pResource->m_iResource);

	UnMapResource(pGlyphResource.get(),subresource);
}
//--------------------------------------------------------------------------------
D3D11_MAPPED_SUBRESOURCE PipelineManager::MapResource(Dx11Resource* pArkResource,UINT subresource,D3D11_MAP actions,UINT flags)
{
	D3D11_MAPPED_SUBRESOURCE Data;
	Data.pData = NULL;
	Data.DepthPitch = Data.RowPitch = 0;

	if(nullptr == pArkResource) {
		ArkLog::Get(LogType::Renderer).Output(L"Trying to map a subresource that doesn't exist!!!");
		return(Data);
	}
	// TODO: Update this to use a ComPtr!
	// Acquire the native resource pointer.
	ID3D11Resource* pResource = 0;
	pResource = pArkResource->GetResource();

	if(nullptr == pResource) {
		ArkLog::Get(LogType::Renderer).Output(L"Trying to map a subresource that has no native resource in it!!!");
		return(Data);
	}

	// Perform the mapping of the resource.
	HRESULT hr = m_pContext->Map(pResource,subresource,actions,flags,&Data);

	if(FAILED(hr)) {
		ArkLog::Get(LogType::Renderer).Output(L"Failed to map resource!");
	}

	return(Data);
}
//--------------------------------------------------------------------------------
void PipelineManager::UnMapResource(Dx11Resource* pArkResource,UINT subresource)
{
	if(NULL == pArkResource) {
		ArkLog::Get(LogType::Renderer).Output(L"Trying to unmap a subresource that doesn't exist!!!");
		return;
	}

	// Acquire the native resource pointer.
	ID3D11Resource* pResource = 0;
	pResource = pArkResource->GetResource();

	if(NULL == pResource) {
		ArkLog::Get(LogType::Renderer).Output(L"Trying to unmap a subresource that has no native resource in it!!!");
		return;
	}

	// Unmap the resource - there is no HRESULT returned, so trust that it works...
	m_pContext->Unmap(pResource,subresource);
}
//--------------------------------------------------------------------------------
void PipelineManager::ResolveSubresource( ResourcePtr DestResource, UINT DstSubresource, 
                                              ResourcePtr SrcResource, UINT SrcSubresource, 
                                              DXGI_FORMAT format )
{
    int DestID = DestResource->m_iResource;
    ID3D11Resource* pDestResource = ArkRenderer11::Get()->GetResourceByIndex(DestID)->GetResource();

    int SrcID = SrcResource->m_iResource;
    ID3D11Resource* pSrcResource = ArkRenderer11::Get()->GetResourceByIndex(SrcID)->GetResource();
 
    m_pContext->ResolveSubresource( pDestResource, DstSubresource, pSrcResource, SrcSubresource, format );
}
//--------------------------------------------------------------------------------
void PipelineManager::GenerateCommandList(ArkCommandList11* plist)
{
	if(m_pContext->GetType() == D3D11_DEVICE_CONTEXT_DEFERRED)
	{
		m_pContext->FinishCommandList(false,&plist->m_pList);

		// Reset the cached context state to default, since we do that for all
		// command lists.

		InputAssemblerStage.ClearPreviousState();
		InputAssemblerStage.ClearCurrentState();

		VertexShaderStage.ClearPreviousState();
		VertexShaderStage.ClearCurrentState();

		HullShaderStage.ClearPreviousState();
		HullShaderStage.ClearCurrentState();

		DomainShaderStage.ClearPreviousState();
		DomainShaderStage.ClearCurrentState();

		GeometryShaderStage.ClearPreviousState();
		GeometryShaderStage.ClearCurrentState();

		StreamOutputStage.ClearPreviousState();
		StreamOutputStage.ClearCurrentState();

		RasterizerStage.ClearPreviousState();
		RasterizerStage.ClearCurrentState();

		PixelShaderStage.ClearPreviousState();
		PixelShaderStage.ClearCurrentState();

		OutputMergerStage.ClearPreviousState();
		OutputMergerStage.ClearCurrentState();

		ComputeShaderStage.ClearPreviousState();
		ComputeShaderStage.ClearCurrentState();

	}
}
//--------------------------------------------------------------------------------
void PipelineManager::ExecuteCommandList(ArkCommandList11* pList)
{
	if(pList->ListAvailable())
		m_pContext->ExecuteCommandList(pList->m_pList,false);

	InputAssemblerStage.ClearPreviousState();
	InputAssemblerStage.ClearCurrentState();

	VertexShaderStage.ClearPreviousState();
	VertexShaderStage.ClearCurrentState();


	HullShaderStage.ClearPreviousState();
	HullShaderStage.ClearCurrentState();

	DomainShaderStage.ClearPreviousState();
	DomainShaderStage.ClearCurrentState();

	GeometryShaderStage.ClearPreviousState();
	GeometryShaderStage.ClearCurrentState();

	StreamOutputStage.ClearPreviousState();
	StreamOutputStage.ClearCurrentState();

	RasterizerStage.ClearPreviousState();
	RasterizerStage.ClearCurrentState();

	PixelShaderStage.ClearPreviousState();
	PixelShaderStage.ClearCurrentState();

	OutputMergerStage.ClearPreviousState();
	OutputMergerStage.ClearCurrentState();

	ComputeShaderStage.ClearPreviousState();
	ComputeShaderStage.ClearCurrentState();
}
//--------------------------------------------------------------------------------
void PipelineManager::BeginEvent(std::wstring& name)
{
	if(m_pAnnotation)
	{
		m_pAnnotation->BeginEvent(name.c_str());
	}
	else
	{
		D3DPERF_BeginEvent(0xFFFFFFFF,name.c_str());
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::EndEvent()
{
	if(m_pAnnotation)
	{
		m_pAnnotation->EndEvent();
	}
	else
	{
		D3DPERF_EndEvent();
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::SetMarker(std::wstring& name)
{
	if(m_pAnnotation)
	{
		m_pAnnotation->SetMarker(name.c_str());
	}
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawIndexed(UINT IndexCount,UINT StartIndex,int VertexOffset)
{
	m_pContext->DrawIndexed(IndexCount,StartIndex,VertexOffset);
}
//--------------------------------------------------------------------------------
void PipelineManager::Draw(UINT VertexCount,UINT StartVertex)
{
	m_pContext->Draw(VertexCount,StartVertex);
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawIndexedInstanced(UINT IndexCountPerInstance,UINT InstanceCount,UINT StartIndexLocation,INT BaseVertexLocation,UINT StartInstanceLocation)
{
	m_pContext->DrawIndexedInstanced(IndexCountPerInstance,InstanceCount,StartIndexLocation,BaseVertexLocation,StartInstanceLocation);
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawInstancedIndirect(ID3D11Buffer* argsBuffer,UINT offset)
{
	m_pContext->DrawInstancedIndirect(argsBuffer,offset);
}
//--------------------------------------------------------------------------------
void PipelineManager::Draw(ArkRenderEffect11& effect,GeometryPtr geometry,
	IParameterManager* pParamManager)
{
	Draw(effect,geometry->m_VB,geometry->m_IB,
		geometry->GetInputLayout(effect.GetVertexShader()),geometry->m_ePrimType,
		geometry->GetVertexSize(),geometry->GetIndexCount(),pParamManager);
}
//--------------------------------------------------------------------------------
void PipelineManager::Draw(ArkRenderEffect11& effect,ResourcePtr vb,ResourcePtr ib,
	int inputLayout,D3D11_PRIMITIVE_TOPOLOGY primType,
	UINT vertexStride,UINT numIndices,IParameterManager* pParamManager)
{
	InputAssemblerStage.ClearCurrentState();

	InputAssemblerStage.CurrentState.PrimitiveTopology.SetState(primType);

	if(vb != NULL)
	{
		InputAssemblerStage.CurrentState.VertexBuffers.SetState(0,vb->m_iResource);
		InputAssemblerStage.CurrentState.VertexBufferStrides.SetState(0,vertexStride);
		InputAssemblerStage.CurrentState.VertexBufferOffsets.SetState(0,0);
	}
	if(ib != NULL)
	{
		InputAssemblerStage.CurrentState.IndexBuffer.SetState(ib->m_iResource);
		InputAssemblerStage.CurrentState.IndexBufferFormat.SetState(DXGI_FORMAT_R32_UINT);
	}

	if(vb != NULL)
	{
		InputAssemblerStage.CurrentState.InputLayout.SetState(inputLayout);
	}

	InputAssemblerStage.ApplyCurrentState(m_pContext.Get());

	ClearPipelineResources();
	effect.ConfigurePipeline(this,pParamManager);
	ApplyPipelineResources();

	m_pContext->DrawIndexed(numIndices,0,0);
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawNonIndexed(ArkRenderEffect11& effect,ResourcePtr vb,
	int inputLayout,D3D11_PRIMITIVE_TOPOLOGY primType,
	UINT vertexStride,UINT vertexCount,UINT startVertexLocation,
	IParameterManager* pParamManager)
{
	InputAssemblerStage.ClearCurrentState();

	// Specify the type of geometry that we will be dealing with.
	InputAssemblerStage.CurrentState.PrimitiveTopology.SetState(primType);

	// Bind the vertex buffer.
	InputAssemblerStage.CurrentState.VertexBuffers.SetState(0,vb->m_iResource);
	InputAssemblerStage.CurrentState.VertexBufferStrides.SetState(0,vertexStride);
	InputAssemblerStage.CurrentState.VertexBufferOffsets.SetState(0,0);

	// Bind the input layout.
	InputAssemblerStage.CurrentState.InputLayout.SetState(inputLayout);

	// Set and apply the state in this pipeline manager's input assembler.
	InputAssemblerStage.ApplyCurrentState(m_pContext.Get());

	// Use the effect to load all of the pipeline stages here.
	ClearPipelineResources();
	effect.ConfigurePipeline(this,pParamManager);
	ApplyPipelineResources();

	m_pContext->Draw(vertexCount,startVertexLocation);
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawInstanced(ArkRenderEffect11& effect,GeometryPtr geometry,
	ResourcePtr instanceData,UINT instanceDataStride,
	UINT numInstances,IParameterManager* pParamManager)
{
	DrawInstanced(effect,geometry->m_VB,geometry->m_ePrimType,geometry->m_IB,
		geometry->GetInputLayout(effect.GetVertexShader()),
		geometry->GetVertexSize(),geometry->GetIndexCount(),
		instanceData,instanceDataStride,numInstances,pParamManager);
}
//--------------------------------------------------------------------------------
void PipelineManager::DrawInstanced(ArkRenderEffect11& effect,ResourcePtr vb,
	D3D11_PRIMITIVE_TOPOLOGY primType,ResourcePtr ib,
	int inputLayout,UINT vertexStride,UINT numIndices,
	ResourcePtr instanceData,UINT instanceDataStride,
	UINT numInstances,IParameterManager* pParamManager)
{
	InputAssemblerStage.ClearCurrentState();

	// Specify the type of geometry that we will be dealing with.
	InputAssemblerStage.CurrentState.PrimitiveTopology.SetState(primType);

	// Bind the vertex buffers.
	InputAssemblerStage.CurrentState.VertexBuffers.SetState(0,vb->m_iResource);
	InputAssemblerStage.CurrentState.VertexBufferStrides.SetState(0,vertexStride);
	InputAssemblerStage.CurrentState.VertexBufferOffsets.SetState(0,0);

	InputAssemblerStage.CurrentState.VertexBuffers.SetState(1,instanceData->m_iResource);
	InputAssemblerStage.CurrentState.VertexBufferStrides.SetState(1,instanceDataStride);
	InputAssemblerStage.CurrentState.VertexBufferOffsets.SetState(1,0);

	// Bind the index buffer.
	InputAssemblerStage.CurrentState.IndexBuffer.SetState(ib->m_iResource);
	InputAssemblerStage.CurrentState.IndexBufferFormat.SetState(DXGI_FORMAT_R32_UINT);

	// Bind the input layout.
	InputAssemblerStage.CurrentState.InputLayout.SetState(inputLayout);

	// Set and apply the state in this pipeline manager's input assembler.
	InputAssemblerStage.ApplyCurrentState(m_pContext.Get());

	// Use the effect to load all of the pipeline stages here.

	ClearPipelineResources();
	effect.ConfigurePipeline(this,pParamManager);
	ApplyPipelineResources();

	m_pContext->DrawIndexedInstanced(numIndices,numInstances,0,0,0);
}
//--------------------------------------------------------------------------------
void PipelineManager::Dispatch( ArkRenderEffect11& effect, UINT x, UINT y, UINT z, IParameterManager* pParamManager )
{
	ClearPipelineResources();
	effect.ConfigurePipeline( this, pParamManager );
	ApplyPipelineResources();

	m_pContext->Dispatch( x, y, z );
}
//--------------------------------------------------------------------------------
void PipelineManager::StartPipelineStatistics()
{
	if(m_Queries[m_iCurrentQuery])
		m_pContext->Begin(m_Queries[m_iCurrentQuery].Get());
	else
		ArkLog::Get(LogType::Renderer).Output(L"Tried to begin pipeline statistics without a query object!");
}
//--------------------------------------------------------------------------------
void PipelineManager::EndPipelineStatistics()
{
	if(m_Queries[m_iCurrentQuery])
	{
		m_pContext->End(m_Queries[m_iCurrentQuery].Get());

		m_iCurrentQuery = (m_iCurrentQuery + 1) % NumQueries;
		HRESULT hr = m_pContext->GetData(m_Queries[m_iCurrentQuery].Get(),&m_PipelineStatsData,
			sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS),0);
		if(FAILED(hr))
			ArkLog::Get(LogType::Renderer).Output(L"Failed attempting to retrieve query data");
	}
	else
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tried to end pipeline statistics without a valid query object!");
	}
}
//--------------------------------------------------------------------------------
std::wstring PipelineManager::PrintPipelineStatistics()
{
	std::wstringstream s;
	s << "Pipeline Statistics:" << std::endl;
	s << "Number of vertices read by the IA: " << m_PipelineStatsData.IAVertices << std::endl;
	s << "Number of primitives read by the IA: " << m_PipelineStatsData.IAPrimitives << std::endl;
	s << "Number of vertex shader invocations: " << m_PipelineStatsData.VSInvocations << std::endl;
	s << "Number of hull shader invocations: " << m_PipelineStatsData.HSInvocations << std::endl;
	s << "Number of domain shader invocations: " << m_PipelineStatsData.DSInvocations << std::endl;
	s << "Number of geometry shader invocations: " << m_PipelineStatsData.GSInvocations << std::endl;
	s << "Number of primitives output by the geometry shader: " << m_PipelineStatsData.GSPrimitives << std::endl;
	s << "Number of primitives sent to the rasterizer: " << m_PipelineStatsData.CInvocations << std::endl;
	s << "Number of primitives rendered: " << m_PipelineStatsData.CPrimitives << std::endl;
	s << "Number of pixel shader invocations: " << m_PipelineStatsData.PSInvocations << std::endl;

	return(s.str());
}
//--------------------------------------------------------------------------------