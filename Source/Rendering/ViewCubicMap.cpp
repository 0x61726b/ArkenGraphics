//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewCubicMap.h"
#include "ArkEntity3D.h"
#include "Scene.h"
#include "Dx11Texture2DConfig.h"
#include "ArkLog.h"
#include "IParameterManager.h"
#include "PipelineManager.h"
#include "Dx11Texture2D.h"
#include "Dx11DepthStencilViewConfig.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11ShaderResourceViewConfig.h"
#include "Dx11SamplerStateConfig.h"
#include "Dx11DepthStencilView.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewCubicMap::ViewCubicMap(ArkRenderer11& Renderer,const ViewSettings& Settings)
{

	ViewMatrix = DirectX::XMMatrixIdentity();
	ProjMatrix = DirectX::XMMatrixIdentity();

	BackBuffer = Settings.BackBuffer;
	DepthBuffer = Settings.DepthBuffer;



	D3D11_VIEWPORT ShadowMapViewport;
	ShadowMapViewport.TopLeftX = 0.0f;
	ShadowMapViewport.TopLeftY = 0.0f;
	ShadowMapViewport.Width = (FLOAT)this->SMSize;
	ShadowMapViewport.Height = (FLOAT)this->SMSize;
	ShadowMapViewport.MinDepth = 0.0f;
	ShadowMapViewport.MaxDepth = 1.0f;

	m_iShadowMapViewport = Renderer.CreateViewport(ShadowMapViewport);
	SetViewport(m_iShadowMapViewport);



	Dx11SamplerStateConfig descSampler;
	descSampler.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.MipLODBias = 0;
	descSampler.MaxAnisotropy = 1;
	descSampler.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = 0.0f;
	descSampler.MaxLOD = 0.0f;

	LightPosWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightPosWS"));
	LightViewProjMatricesParameter = Renderer.m_pParamMgr->GetMatrixArrayParameterRef(std::wstring(L"LightViewProjMatrices"),6);
	ShadowMapCubeParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"ShadowMap"));
	LightRangeParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightRange"));

	SetupLights();
}
//--------------------------------------------------------------------------------
ViewCubicMap::~ViewCubicMap()
{
}
//--------------------------------------------------------------------------------
void ViewCubicMap::QueuePreTasks(ArkRenderer11* pRenderer)
{
	if(m_pEntity != NULL)
	{
		XMMATRIX view = m_pEntity->Transform.GetView();
		SetViewMatrix(view);
	}

	// Queue this view into the renderer for processing.
	pRenderer->QueueTask(this);

	if(m_pScene)
	{
		// Run through the graph and pre-render the entities
		m_pScene->PreRender(pRenderer,VT_LINEAR_DEPTH_NORMAL);
	}
}
//--------------------------------------------------------------------------------
void ViewCubicMap::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->BeginEvent(std::wstring(L"Depth Pass"));



	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,-1);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(BackBuffer->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={vColor.x,vColor.y,vColor.z,vColor.w};
	Dx11DepthStencilView& dsv = ArkRenderer11::Get()->GetDepthStencilViewByIndex(BackBuffer->m_iResourceDSV);
	ID3D11DepthStencilView* pShadowMapCubeDepthView = dsv.m_pDepthStencilView.Get();
	pPipelineManager->m_pContext->ClearDepthStencilView(pShadowMapCubeDepthView,D3D11_CLEAR_DEPTH,1.0f,0);



	SetViewport(m_iShadowMapViewport);
	ConfigureViewports(pPipelineManager);

	pParamManager->SetMatrixArrayParameter(LightViewProjMatricesParameter,6,m_mLightView4x4);
	pParamManager->SetVectorParameter(LightPosWSParameter,&m_vLightPosition);
	float lightRange = 10;
	pParamManager->SetVectorParameter(LightRangeParameter,&XMVectorSet(lightRange,lightRange,lightRange,lightRange));

	m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_LINEAR_DEPTH_NORMAL);


	pPipelineManager->EndEvent();
}
//--------------------------------------------------------------------------------
void ViewCubicMap::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}
//--------------------------------------------------------------------------------
void ViewCubicMap::SetUsageParams(IParameterManager* pParamManager)
{
	// This view will bind the depth/normal buffer to the "DepthNormalBuffer" shader
	// resource view parameter, so that other views can make use of it.
	pParamManager->SetShaderResourceParameter(ShadowMapCubeParameter,ShadowMap);
}
//--------------------------------------------------------------------------------
void ViewCubicMap::Update(float dt)
{
}
//--------------------------------------------------------------------------------
void ViewCubicMap::Resize(UINT width,UINT height)
{
	//ArkRenderer11::Get()->ResizeTexture( m_pDepthTarget,width,height );
	//ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
std::wstring ViewCubicMap::GetName()
{
	return(L"ViewCubicMap");
}
//--------------------------------------------------------------------------------
void ViewCubicMap::SetupLights()
{
	m_vLightPosition = XMVectorSet(0,10.0f,0,1.0f);
	m_mLightProjMatrix = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV2,(FLOAT)this->SMSize /(FLOAT)this->SMSize,0.1f,40.0f);

	float range = 20;

	m_mLightViewMatrices[0] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(range,0.0f,0.0f,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f)) * m_mLightProjMatrix;
	m_mLightViewMatrices[1] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(-range,0.0f,0.0f,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f)) * m_mLightProjMatrix;
	m_mLightViewMatrices[2] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(0.0f,range,0.0f,0.0f),XMVectorSet(0.0f,0.0f,-1.0f,0.0f)) * m_mLightProjMatrix;
	m_mLightViewMatrices[3] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(0.0f,-range,0.0f,0.0f),XMVectorSet(0.0f,0.0f,1.0f,0.0f)) * m_mLightProjMatrix;
	m_mLightViewMatrices[4] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(0.0f,0.0f,range,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f)) * m_mLightProjMatrix;
	m_mLightViewMatrices[5] = XMMatrixLookAtLH(m_vLightPosition,m_vLightPosition + XMVectorSet(0.0f,0.0f,-range,0.0f),XMVectorSet(0.0f,1.0f,0.0f,0.0f)) * m_mLightProjMatrix;

	m_mLightView4x4 = new XMFLOAT4X4[6];
	for(int i=0; i < 6; ++i)
	{
		XMFLOAT4X4 m;
		XMStoreFloat4x4(&m,m_mLightViewMatrices[i]);
		m_mLightView4x4[i] = m;

	}
}
//--------------------------------------------------------------------------------