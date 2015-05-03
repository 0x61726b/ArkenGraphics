//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewShadowMap.h"
#include "ArkEntity3D.h"
#include "ArkNode3D.h"
#include "Dx11Texture2DConfig.h"
#include "ArkLog.h"
//#include "ActorGenerator.h"
#include "IParameterManager.h"
#include "Scene.h"
#include "Dx11BlendStateConfig.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11DepthStencilStateConfig.h"
#include "Dx11SamplerStateConfig.h"
#include "ArkMath.h"
#include "Camera.h"
#include "ArkFrustum.h"
#include "ArkBuffer11Config.h"
#include "ArkRenderParameter11.h"
#include "Dx11DepthStencilViewConfig.h"
#include "Dx11ShaderResourceViewConfig.h"
#include "OrthoCamera.h"
#include "Dx11Texture2D.h"
#include "Dx11DepthStencilView.h"
#include "Dx11ViewPort.h"
#include "Dx11RenderTargetViewConfig.h"
#include "ArkGeometryGenerator11.h"
#include "ArkGeometry11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;

static const XMFLOAT3 LightDir = XMFLOAT3(0.557f,0.557f,0.557f);
static const XMFLOAT3 LightColor = XMFLOAT3(10,5,8);
//--------------------------------------------------------------------------------
ViewShadowMap::ViewShadowMap(ArkRenderer11& Renderer,ResourcePtr RenderTarget,ResourcePtr d)
{
	BackBuffer = RenderTarget;
	DepthTarget = d;

	ViewMatrix = DirectX::XMMatrixIdentity();
	ProjMatrix = DirectX::XMMatrixIdentity();

#pragma region Resource and stuff
	D3D11_TEXTURE2D_DESC desc = BackBuffer->m_pTexture2dConfig->GetTextureDesc();

	int ResolutionX = desc.Width;
	int ResolutionY = desc.Height;


	Dx11BlendStateConfig BlendConfig;
	BlendConfig.AlphaToCoverageEnable = false;
	BlendConfig.IndependentBlendEnable = false;
	for(UINT i = 0; i < 8; ++i)
	{
		BlendConfig.RenderTarget[i].BlendEnable = false;
		BlendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendConfig.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	iBlendDisabledState = Renderer.CreateBlendState(&BlendConfig);

	for(UINT i = 0; i < 8; ++i)
	{
		BlendConfig.RenderTarget[i].BlendEnable = false;
		BlendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendConfig.RenderTarget[i].RenderTargetWriteMask = 0;
		BlendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	iColorWriteDisabledBSS = Renderer.CreateBlendState(&BlendConfig);

	Dx11RasterizerStateConfig RasterizerConfig;
	RasterizerConfig.AntialiasedLineEnable = FALSE;
	RasterizerConfig.CullMode = D3D11_CULL_BACK;
	RasterizerConfig.DepthBias = 0;
	RasterizerConfig.DepthBiasClamp = 0.0f;
	RasterizerConfig.DepthClipEnable = TRUE;
	RasterizerConfig.FillMode = D3D11_FILL_SOLID;
	RasterizerConfig.FrontCounterClockwise = false;
	RasterizerConfig.MultisampleEnable = true;
	RasterizerConfig.ScissorEnable = false;
	RasterizerConfig.SlopeScaledDepthBias = 0;

	iBackFaceCullRS = Renderer.CreateRasterizerState(&RasterizerConfig);

	Dx11DepthStencilStateConfig DSSConfig;
	DSSConfig.DepthEnable = true;
	DSSConfig.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSSConfig.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	DSSConfig.StencilEnable = false;
	DSSConfig.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DSSConfig.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	DSSConfig.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSSConfig.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSSConfig.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	DSSConfig.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	DSSConfig.BackFace = DSSConfig.FrontFace;

	iDepthWriteEnabledDSS = Renderer.CreateDepthStencilState(&DSSConfig);

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.Filter = D3D11_FILTER_ANISOTROPIC;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.MipLODBias = 0.0f;
	SamplerConfig.MaxAnisotropy = 16;
	SamplerConfig.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerConfig.BorderColor[0] = SamplerConfig.BorderColor[1] = SamplerConfig.BorderColor[2] = SamplerConfig.BorderColor[3] = 0;
	SamplerConfig.MinLOD = 0;
	SamplerConfig.MaxLOD = D3D11_FLOAT32_MAX;

	iAnisoSampler = Renderer.CreateSamplerState(&SamplerConfig);

	SamplerConfig.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.MipLODBias = 0.0f;
	SamplerConfig.MaxAnisotropy = 1;
	SamplerConfig.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	SamplerConfig.BorderColor[0] = SamplerConfig.BorderColor[1] = SamplerConfig.BorderColor[2] = SamplerConfig.BorderColor[3] = 0;
	SamplerConfig.MinLOD = 0;
	SamplerConfig.MaxLOD = D3D11_FLOAT32_MAX;

	iShadowMapSampler = Renderer.CreateSamplerState(&SamplerConfig);

	Dx11DepthStencilViewConfig DSVConfig;
	D3D11_TEX2D_DSV DSVTex2D;
	DSVTex2D.MipSlice = 0;
	DSVConfig.SetTexture2D(DSVTex2D);
	DSVConfig.SetFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	DSVConfig.SetFlags(0);
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count =1;
	sampleDesc.Quality = 0;

	int rtWidth = ResolutionX;
	int rtHeight = ResolutionY;


	Dx11Texture2DConfig DepthTexConfig;
	DepthTexConfig.SetWidth(rtWidth);
	DepthTexConfig.SetHeight(rtWidth);
	DepthTexConfig.SetArraySize(1);
	DepthTexConfig.SetBindFlags(D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
	DepthTexConfig.SetSampleDesc(sampleDesc);
	DepthTexConfig.SetMipLevels(1);
	DepthTexConfig.SetFormat(DXGI_FORMAT_R24G8_TYPELESS);

	Dx11ShaderResourceViewConfig DepthSRVConfig;
	D3D11_TEX2D_ARRAY_SRV tx2DSrvArray;
	tx2DSrvArray.ArraySize = 1;
	tx2DSrvArray.FirstArraySlice = 0;
	tx2DSrvArray.MipLevels = 1;
	DepthSRVConfig.SetTexture2DArray(tx2DSrvArray);

	D3D11_TEX2D_SRV SRVTex2D;
	SRVTex2D.MipLevels = 1;
	SRVTex2D.MostDetailedMip = 0;
	DepthSRVConfig.SetTexture2D(SRVTex2D);
	DepthSRVConfig.SetFormat(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

	DepthSRVConfig.SetViewDimensions(D3D11_SRV_DIMENSION_TEXTURE2D);
	DSVConfig.SetViewDimensions(D3D11_DSV_DIMENSION_TEXTURE2D);

	ShadowMap = Renderer.CreateTexture2D(&DepthTexConfig,NULL,&DepthSRVConfig,NULL,NULL,&DSVConfig);



	Dx11Texture2DConfig DepthTexConfig2;
	DepthTexConfig2.SetDepthBuffer(rtWidth,rtHeight);
	DepthTexConfig2.SetFormat(DXGI_FORMAT_R24G8_TYPELESS);
	DepthTexConfig2.SetBindFlags(D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
	DepthTexConfig2.SetSampleDesc(sampleDesc);

	Dx11DepthStencilViewConfig DepthDSVConfig2;
	DSVTex2D.MipSlice = 0;
	DepthDSVConfig2.SetTexture2D(DSVTex2D);
	DepthDSVConfig2.SetFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);


	Dx11ShaderResourceViewConfig DepthSRVConfig2;
	D3D11_TEX2D_SRV SRVTex2D2;
	SRVTex2D2.MipLevels = 1;
	SRVTex2D2.MostDetailedMip = 0;
	DepthSRVConfig2.SetTexture2D(SRVTex2D2);
	DepthSRVConfig2.SetFormat(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);



	DepthSRVConfig2.SetViewDimensions(D3D11_SRV_DIMENSION_TEXTURE2D);
	DepthDSVConfig2.SetViewDimensions(D3D11_DSV_DIMENSION_TEXTURE2D);



	DepthTarget = Renderer.CreateTexture2D(&DepthTexConfig2,NULL,&DepthSRVConfig2,NULL,NULL,&DepthDSVConfig2);
	Dx11Texture2DConfig DepthConfig;
	DepthConfig.SetDepthBuffer(rtWidth,rtHeight);
	DepthTarget = ArkRenderer11::Get()->CreateTexture2D(&DepthConfig,0);

	Dx11Texture2DConfig RTVTexConfig;
	RTVTexConfig.SetWidth(rtWidth);
	RTVTexConfig.SetHeight(rtHeight);
	RTVTexConfig.SetArraySize(1);
	RTVTexConfig.SetBindFlags(D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET);
	RTVTexConfig.SetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	RTVTexConfig.SetMipLevels(1);
	DXGI_SAMPLE_DESC RTVSampleDesc;
	RTVSampleDesc.Count = 1;
	RTVSampleDesc.Quality = 0;
	RTVTexConfig.SetSampleDesc(RTVSampleDesc);

	/*BackBuffer = Renderer.CreateTexture2D( &RTVTexConfig,0);*/
#pragma end region



	AnisoSamplerParameter = Renderer.m_pParamMgr->GetSamplerStateParameterRef(std::wstring(L"AnisoSampler"));
	ShadowMapSamplerParameter = Renderer.m_pParamMgr->GetSamplerStateParameterRef(std::wstring(L"ShadowSampler"));

	LightColorParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightColor"));
	LightDirWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightDirWS"));

	CamPosWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"CameraPosWS"));

	ShadowMapSrvParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"ShadowMap"));

	LightViewMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"LightViewMatrix"));
	LightProjMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"LightProjMatrix"));

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(desc.Width);
	viewport.Height = static_cast<float>(desc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	SetViewport(ArkRenderer11::Get()->CreateViewport(viewport));

	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());
	pDepthEffect = std::make_shared<ArkRenderEffect11>();
	pDepthEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"DepthVS.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")
		));
	pDepthEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"DepthVS.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")
		));


	pMainEffect = std::make_shared<ArkRenderEffect11>();
	pMainEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"ShadowMap.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_5_0")
		));
	pMainEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"ShadowMap.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_5_0")
		));

	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pMainEffect;


	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;

}
//--------------------------------------------------------------------------------
ViewShadowMap::~ViewShadowMap()
{
}
//--------------------------------------------------------------------------------
void ViewShadowMap::Resize(UINT width,UINT height)
{
	// The resources themselves will be resized by the parent view, but we just
	// need to record how big of a render target we will be processing.
	//ResolutionX = width;
	//ResolutionY = height;
	ArkRenderer11::Get()->ResizeTexture(DepthTarget,width,height);
	ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
//--------------------------------------------------------------------------------
void ViewShadowMap::Update(float fTime)
{
}
//--------------------------------------------------------------
void ViewShadowMap::QueuePreTasks(ArkRenderer11* pRenderer)
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
		m_pScene->PreRender(pRenderer,VT_PERSPECTIVE);
	}
}
//--------------------------------------------------------------------------------
void ViewShadowMap::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,BackBuffer->m_iResourceRTV);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(DepthTarget->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={0,0,0,0};
	pPipelineManager->ClearBuffers(color,1.0f);

	ConfigureViewports(pPipelineManager);


	pPipelineManager->ClearPipelineResources();


	pPipelineManager->RasterizerStage.CurrentState.RasterizerState.SetState(iBackFaceCullRS);
	//Render Depth Buffer

	int prevRenderTarget = pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.GetState(0);
	int prevDepthStencil = pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.GetState();
	int prevViewport = pPipelineManager->RasterizerStage.CurrentState.ViewPorts.GetState(0);
	int prevRSS = pPipelineManager->RasterizerStage.CurrentState.RasterizerState.GetState();


	pPipelineManager->RasterizerStage.CurrentState.RasterizerState.SetState(iBackFaceCullRS);

	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,-1);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(ShadowMap->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();
	pPipelineManager->ClearBuffers(new float[4] { 0,0,0,1 },1.0f);

	pDepthEffect->m_iBlendState = iColorWriteDisabledBSS;
	pDepthEffect->m_iDepthStencilState = iDepthWriteEnabledDSS;

	XMVECTOR eye = XMVectorSet(10,20,10,1.0f);
	XMVECTOR target = XMVectorSet(0,0,0,0);
	XMMATRIX view = XMMatrixLookAtLH(eye,target,XMVectorSet(0,1,0,0));
	XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4,1280/720,0.01f,40.0f);

	pParamManager->SetMatrixParameter(LightViewMatrix,&view);
	pParamManager->SetMatrixParameter(LightProjMatrix,&proj);

	XMMATRIX View = view;
	XMMATRIX Proj = proj;
	/*pParamManager->SetWorldMatrix(&(world));*/
	pParamManager->SetViewMatrix(&(View));
	pParamManager->SetProjectionMatrix(&(Proj));

	m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_LINEAR_DEPTH_NORMAL);

	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,prevRenderTarget);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(prevDepthStencil);
	pPipelineManager->ApplyRenderTargets();


	pPipelineManager->RasterizerStage.CurrentState.ViewPorts.SetState(0,prevViewport);
	pPipelineManager->RasterizerStage.CurrentState.RasterizerState.SetState(prevRSS);
	pPipelineManager->ApplyPipelineResources();
	//





	//Render Normal

	pMainEffect->m_iBlendState = iBlendDisabledState;
	pMainEffect->m_iDepthStencilState = iDepthWriteEnabledDSS;
	pMainEffect->m_iRasterizerState = iBackFaceCullRS;

	pParamManager->SetSamplerParameter(AnisoSamplerParameter,&iAnisoSampler);
	pParamManager->SetSamplerParameter(ShadowMapSamplerParameter,&iShadowMapSampler);

	XMVECTOR lightDirV,lightColor;
	lightColor = XMVectorSet(10.0f,8.0f,5.0f,1.0f);
	lightDirV = XMVector3Normalize(XMVectorSet(0.577f,0.577f,0.577f,0.0f));
	pParamManager->SetVectorParameter(LightColorParameter,&lightColor);
	pParamManager->SetVectorParameter(LightDirWSParameter,&lightDirV);



	view = m_pScene->GetCamera()->GetBody()->Transform.GetView();
	proj = m_pScene->GetCamera()->m_ProjMatrix;

	
	XMMATRIX m = m_pScene->GetCamera()->GetBody()->Transform.WorldMatrix();

	XMVECTOR scale;
	XMVECTOR translation;
	XMVECTOR rotation;
	XMMatrixDecompose(&scale,&rotation,&translation,m);

	pParamManager->SetViewMatrix(&(view));
	pParamManager->SetProjectionMatrix(&(proj));

	pParamManager->SetVectorParameter(CamPosWSParameter,&translation);



	pParamManager->SetShaderResourceParameter(ShadowMapSrvParameter,ShadowMap);
	m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_PERSPECTIVE);

}

//--------------------------------------------------------------------------------
void ViewShadowMap::SetUsageParams(IParameterManager* pParamManager)
{

}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetScene(Scene* pScene)
{
	m_pScene = pScene;

	ArkNode3D* Root = m_pScene->GetRoot();
	std::vector<ArkNode3D*> Nodes = Root->Nodes();

	for(int i=0; i < Nodes.size(); ++i)
	{
		ArkNode3D* ActorNodes = Nodes[i];
		std::vector<ArkEntity3D*> Leafs = ActorNodes->Leafs();
		for(int j=0; j < Leafs.size(); ++j)
		{
			ArkEntity3D* Entity = Leafs[j];

			if(Entity->Visual.Executor != nullptr)
			{

				if(Entity->Visual.Material != nullptr)
				{
					MaterialPtr pMaterial = Entity->Visual.Material;
					pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;
					pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;

					pMaterial->Params[VT_PERSPECTIVE].pEffect = pMainEffect;
					pMaterial->Params[VT_PERSPECTIVE].bRender = true;
				}
			}

		}
	}
}
std::wstring ViewShadowMap::GetName()
{
	return(L"ViewShadowMap");
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetViewMatrix(const XMMATRIX& View)
{
	RenderTask::SetViewMatrix(View);
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetProjMatrix(const XMMATRIX& Proj)
{
	RenderTask::SetProjMatrix(Proj);
}
//--------------------------------------------------------------------------------