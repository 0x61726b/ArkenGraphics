//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewOmnidirectionalShadowMap.h"
#include "PipelineManager.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11Texture2D.h"
#include "Scene.h"
#include "ViewSettings.h"
#include "Dx11DepthStencilViewConfig.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11ShaderResourceViewConfig.h"
#include "Dx11SamplerStateConfig.h"
#include "Dx11DepthStencilView.h"
#include "CSMViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ViewOmnidirectionalShadowMap::ViewOmnidirectionalShadowMap(ArkRenderer11& Renderer,const ViewSettings& Settings)
{
	ViewMatrix = DirectX::XMMatrixIdentity();
	ProjMatrix = DirectX::XMMatrixIdentity();

	BackBuffer = Settings.BackBuffer;
	DepthBuffer = Settings.DepthBuffer;



	D3D11_TEXTURE2D_DESC desc = BackBuffer->m_pTexture2dConfig->GetTextureDesc();

	if(DepthBuffer == NULL)
	{
		Dx11Texture2DConfig DepthConfig;
		DepthConfig.SetDepthBuffer(desc.Width,desc.Height);
		DepthBuffer = ArkRenderer11::Get()->CreateTexture2D(&DepthConfig,0);
	}

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(desc.Width);
	viewport.Height = static_cast<float>(desc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	int iViewport = Renderer.CreateViewport(viewport);

	m_iViewport = iViewport;
	//Setup resources
	Dx11Texture2DConfig ShadowMapTexConfig;
	ShadowMapTexConfig.SetWidth(this->ShadowMapSize);
	ShadowMapTexConfig.SetHeight(this->ShadowMapSize);
	ShadowMapTexConfig.SetMipLevels(1);
	ShadowMapTexConfig.SetArraySize(6);
	ShadowMapTexConfig.SetFormat(DXGI_FORMAT_R32_TYPELESS);
	DXGI_SAMPLE_DESC SampleDesc;
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;
	ShadowMapTexConfig.SetSampleDesc(SampleDesc);
	ShadowMapTexConfig.SetUsage(D3D11_USAGE_DEFAULT);
	ShadowMapTexConfig.SetBindFlags(D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
	ShadowMapTexConfig.SetCPUAccessFlags(0);
	ShadowMapTexConfig.SetMiscFlags(D3D11_RESOURCE_MISC_TEXTURECUBE);

	Dx11DepthStencilViewConfig DSVConfig;
	DSVConfig.SetViewDimensions(D3D11_DSV_DIMENSION_TEXTURE2DARRAY);
	DSVConfig.SetFormat(DXGI_FORMAT_D32_FLOAT);
	DSVConfig.SetFlags(0);
	D3D11_TEX2D_ARRAY_DSV DSVTexArray;
	DSVTexArray.MipSlice = 0;
	DSVTexArray.FirstArraySlice = 0;
	DSVTexArray.ArraySize = 6;
	DSVConfig.SetTexture2DArray(DSVTexArray);


	Dx11ShaderResourceViewConfig SRVConfig;
	SRVConfig.SetFormat(DXGI_FORMAT_R32_FLOAT);
	SRVConfig.SetViewDimensions(D3D11_SRV_DIMENSION_TEXTURECUBE);
	D3D11_TEXCUBE_SRV SRVTexCube;
	SRVTexCube.MipLevels = 1;
	SRVTexCube.MostDetailedMip = 0;
	SRVConfig.SetTextureCube(SRVTexCube);

	ShadowMap = Renderer.CreateTexture2D(&ShadowMapTexConfig,0,&SRVConfig,0,0,&DSVConfig);
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

	ShadowMapSampler = Renderer.CreateSamplerState(&descSampler);

	
	LightPosWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightPosWS"));
	ViewPositionWS = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"ViewPosWS"));
	ViewDirectionWS = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"ViewPosWS"));
	
	LightRangeParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightRange"));
	ShadowMapCubeParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"ShadowMap"));
	ShadowSamplerParameter = Renderer.m_pParamMgr->GetSamplerStateParameterRef(std::wstring(L"ShadowMapSampler"));
	LightColorParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightColor"));


	CSMViewSettings ns(ShadowMap,DepthBuffer);

	m_pViewCubicMap = new ViewCubicMap(Renderer,ns);
}
//--------------------------------------------------------------------------------
ViewOmnidirectionalShadowMap::~ViewOmnidirectionalShadowMap()
{
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::Update(float dt)
{
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::QueuePreTasks(ArkRenderer11* pRenderer)
{
	if(m_pEntity != NULL)
	{
		XMMATRIX view = (m_pEntity->Transform.GetView());
		SetViewMatrix(view);
	}

	pRenderer->QueueTask(this);

	if(m_pScene)
	{
		m_pScene->PreRender(pRenderer,VT_PERSPECTIVE);
	}
	m_pViewCubicMap->QueuePreTasks(pRenderer);
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	/*RenderDepthOnly(pPipelineManager,pParamManager);*/
	RenderNormally(pPipelineManager,pParamManager);
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::RenderDepthOnly(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->BeginEvent(std::wstring(L"Depth Pass"));



	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,-1);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(ShadowMap->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={vColor.x,vColor.y,vColor.z,vColor.w};
	Dx11DepthStencilView& dsv = ArkRenderer11::Get()->GetDepthStencilViewByIndex(ShadowMap->m_iResourceDSV);
	ID3D11DepthStencilView* pShadowMapCubeDepthView = dsv.m_pDepthStencilView.Get();
	pPipelineManager->m_pContext->ClearDepthStencilView(pShadowMapCubeDepthView,D3D11_CLEAR_DEPTH,1.0f,0);

	pParamManager->SetVectorParameter(LightPosWSParameter,&m_vLightPosition);

	SetViewport(m_iShadowMapViewport);
	ConfigureViewports(pPipelineManager);



	pParamManager->SetMatrixArrayParameter(LightViewProjMatricesParameter,6,m_mLightView4x4);

	m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_LINEAR_DEPTH_NORMAL);


	pPipelineManager->EndEvent();
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::RenderNormally(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->BeginEvent(std::wstring(L"Render Base Scene"));
	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,BackBuffer->m_iResourceRTV);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(DepthBuffer->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={vColor.x,vColor.y,vColor.z,vColor.w};
	pPipelineManager->ClearBuffers(color,1.0f);

	SetViewport(m_iViewport);
	ConfigureViewports(pPipelineManager);


	SetRenderParams(pParamManager);
	XMMATRIX view = m_pScene->GetCamera()->GetBody()->Transform.GetView();
	XMMATRIX proj = m_pScene->GetCamera()->m_ProjMatrix;

	m_vLightPosition = XMVectorSet(0,10.0f,0,1.0f);
	pParamManager->SetVectorParameter(LightPosWSParameter,&m_vLightPosition);
	
	float lightRange = 10;
	pParamManager->SetVectorParameter(LightRangeParameter,&XMVectorSet(lightRange,lightRange,lightRange,lightRange));
	pParamManager->SetSamplerParameter(ShadowSamplerParameter,&ShadowMapSampler);
	XMVECTOR lightDirV,lightColor;
	lightColor = XMVectorSet(0.8f,0.6f,0.5f,1.0f);
	pParamManager->SetVectorParameter(LightColorParameter,&lightColor);


	XMMATRIX m = m_pScene->GetCamera()->GetBody()->Transform.WorldMatrix();
	XMVECTOR translate;
	XMVECTOR scale;
	XMVECTOR rot;
	XMMatrixDecompose(&scale,&rot,&translate,m);
	pParamManager->SetVectorParameter(ViewPositionWS,&translate);
	pParamManager->SetShaderResourceParameter(ShadowMapCubeParameter,ShadowMap);

	pPipelineManager->ClearPipelineResources();

	m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_PERSPECTIVE);
	pPipelineManager->EndEvent();
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::Resize(UINT width,UINT height)
{
	//ArkRenderer11::Get()->ResizeTexture( m_pDepthTarget,width,height );
	//ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
//--------------------------------------------------------------------------------
void ViewOmnidirectionalShadowMap::SetScene(Scene* pScene)
{
	m_pScene = pScene;
	m_pViewCubicMap->SetScene(pScene);
}
//--------------------------------------------------------------------------------
std::wstring ViewOmnidirectionalShadowMap::GetName()
{
	return std::wstring(L"ViewOmnidirectionalShadowMap View");
}
