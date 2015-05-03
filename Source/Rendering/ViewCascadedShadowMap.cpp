//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ViewCascadedShadowMap.h"
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
#include "ArkMaterialGenerator11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
static const UINT ShadowMapSize = 2048;
static const float ShadowDist = 1.0f;
static const float Backup = 20.0f;
static const float NearClip = 1.0f;
static const float CascadeSplits[4] ={0.125f,0.25f,0.5f,1.0f};
static const float Bias = 0.005f;
using namespace DirectX;
//--------------------------------------------------------------------------------
ViewCascadedShadowMap::ViewCascadedShadowMap(ArkRenderer11& Renderer,const ViewSettings& Settings)
{
	
	BackBuffer = Settings.BackBuffer;
	DepthTarget = Settings.DepthBuffer;

	ViewMatrix = DirectX::XMMatrixIdentity();
	ProjMatrix = DirectX::XMMatrixIdentity();

#pragma region Resource Creation

	D3D11_TEXTURE2D_DESC desc = BackBuffer->m_pTexture2dConfig->GetTextureDesc();

	ResolutionX = desc.Width;
	ResolutionY = desc.Height;

	//Create Blend States
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
	DepthTexConfig.SetWidth(ShadowMapSize*2);
	DepthTexConfig.SetHeight(ShadowMapSize*2);
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


	//WorldMatrixParameter = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"gWorld"));
	//ViewMatrixParameter = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"gView"));
	//WvPMatrixParameter = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"WorldViewProjMatrix"));
	ShadowMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"ShadowMatrix"));
	ShadowMatrix2 = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"ShadowMatrix2"));
	ShadowMatrix3 = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"ShadowMatrix3"));
	ShadowMatrix4 = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"ShadowMatrix4"));

	ShadowMatrices = Renderer.m_pParamMgr->GetMatrixArrayParameterRef(std::wstring(L"ShadowMatrices"),4);
	CascadeSplitsParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"CascadeSplits"));
	AnisoSamplerParameter = Renderer.m_pParamMgr->GetSamplerStateParameterRef(std::wstring(L"AnisoSampler"));
	ShadowMapSamplerParameter = Renderer.m_pParamMgr->GetSamplerStateParameterRef(std::wstring(L"ShadowSampler"));

	LightColorParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightColor"));
	LightDirWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"LightDirWS"));

	CamPosWSParameter = Renderer.m_pParamMgr->GetVectorParameterRef(std::wstring(L"CameraPosWS"));

	ShadowMapSrvParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"ShadowMap"));


	XMVECTOR lightDirV,lightColor;
	lightColor = XMVectorSet(10.0f,8.0f,5.0f,1.0f);
	lightDirV = XMVector3Normalize(XMVectorSet(0.577f,0.577f,0.577f,0.0f));

	XMStoreFloat3(&lightDir,lightDirV);


	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(desc.Width);
	viewport.Height = static_cast<float>(desc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	const XMFLOAT2 Offsets[4] ={
		XMFLOAT2(0.0f,0.0f),
		XMFLOAT2(0.5f,0.0f),
		XMFLOAT2(0.5f,0.5f),
		XMFLOAT2(0.0f,0.5f)
	};

	for(UINT cascadeIdx = 0; cascadeIdx < NumCascades; ++cascadeIdx)
	{
		D3D11_VIEWPORT cascadeViewport;
		cascadeViewport.TopLeftX = Offsets[cascadeIdx].x * ShadowMapSize * 2;
		cascadeViewport.TopLeftY = Offsets[cascadeIdx].y * ShadowMapSize* 2;
		cascadeViewport.Width = static_cast<float>(ShadowMapSize);
		cascadeViewport.Height = cascadeViewport.Width;
		cascadeViewport.MinDepth = 0.0f;
		cascadeViewport.MaxDepth = 1.0f;

		CascadeViewPorts.push_back(ArkRenderer11::Get()->CreateViewport(cascadeViewport));
	}
	SetViewport(ArkRenderer11::Get()->CreateViewport(viewport));

#pragma endregion

	/*m_pSpriteRenderer.Initialize();*/

	//Traverse actors

	MaterialPtr pMaterial = ArkMaterialGenerator11::GenerateCascadedShadowMaps(Renderer);
	pDepthEffect = pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect;
	pMainEffect  = pMaterial->Params[VT_PERSPECTIVE].pEffect;
}
//--------------------------------------------------------------------------------
ViewCascadedShadowMap::~ViewCascadedShadowMap()
{

}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::InitializeResources()
{
	/*std::vector<Actor*> Actors = m_pScene->GetActors();*/
}
void ViewCascadedShadowMap::Resize(UINT width,UINT height)
{
	// The resources themselves will be resized by the parent view, but we just
	// need to record how big of a render target we will be processing.
	ResolutionX = width;
	ResolutionY = height;
	ArkRenderer11::Get()->ResizeTexture(DepthTarget,width,height);
	ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::Update(float fTime)
{
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::QueuePreTasks(ArkRenderer11* pRenderer)
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

	//std::vector<Actor*> Actors = m_pScene->GetActors();
	//for( int i=0; i < Actors.size(); ++i )
	//{
	//	AddActor(Actors[i] );
	//}
	//m_vActors.clear();
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
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
	
	RenderShadowMap(pPipelineManager,pParamManager);
	RenderSceneNormally(pPipelineManager,pParamManager);

}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::RenderDepth(PipelineManager* pPipelineManager,IParameterManager* pParamManager,const XMMATRIX& view,const XMMATRIX& proj)
{
	DoFrustumTests(view,proj);
	if(numSuccessfulTests == 0)
		return;

	pDepthEffect->m_iBlendState = iColorWriteDisabledBSS;
	pDepthEffect->m_iDepthStencilState = iDepthWriteEnabledDSS;

	
	XMMATRIX View = view;
	XMMATRIX Proj = proj;
	/*pParamManager->SetWorldMatrix(&(world));*/
	pParamManager->SetViewMatrix(&(View));
	pParamManager->SetProjectionMatrix(&(Proj));


	if(frustumTests[0])
		m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_LINEAR_DEPTH_NORMAL);

	//XMMATRIX mat = XMMatrixIdentity();
	//float s = 0.5f * 1;
	//mat = XMMatrixScaling(s,s,s) *  XMMatrixTranslation(0,0,0);
	//XMFLOAT4X4 mat4x4;
	//XMStoreFloat4x4(&mat4x4,mat);
	//m_pSpriteRenderer.Render(pPipelineManager,pParamManager,ShadowMap,mat4x4);

	//4096X4096 SHADOW MAP IS NOT BEING GENERATED,CHECK DEBUG VIEW
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::RenderShadowMap(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->BeginEvent(std::wstring(L"Shadow Pass"));



	XMMATRIX view = m_pScene->GetCamera()->GetBody()->Transform.GetView();
	XMMATRIX proj = m_pScene->GetCamera()->m_ProjMatrix;

	int prevRenderTarget = pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.GetState(0);
	int prevDepthStencil = pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.GetState();
	int prevViewport = pPipelineManager->RasterizerStage.CurrentState.ViewPorts.GetState(0);
	int prevRSS = pPipelineManager->RasterizerStage.CurrentState.RasterizerState.GetState();


	pPipelineManager->RasterizerStage.CurrentState.RasterizerState.SetState(iBackFaceCullRS);

	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,-1);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(ShadowMap->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();
	pPipelineManager->ClearBuffers(new float[4] { 0,0,0,1 },1.0f);

	const XMFLOAT2 Offsets[4] ={
		XMFLOAT2(0.0f,0.0f),
		XMFLOAT2(0.5f,0.0f),
		XMFLOAT2(0.5f,0.5f),
		XMFLOAT2(0.0f,0.5f)
	};

	const float sMapSize = static_cast<float>(ShadowMapSize);

	XMFLOAT4 cSplit;
	XMMATRIX matrixArray;
	std::vector<XMFLOAT4X4> cascadeMatrixArray4x4;
	XMFLOAT4X4* csdMatrixArray4x4 = new XMFLOAT4X4[4];
	std::vector<XMMATRIX> cascadeMatrixArray;
	for(UINT cascadeIdx = 0; cascadeIdx < NumCascades; ++cascadeIdx)
	{
		pPipelineManager->BeginEvent(std::wstring(L"Rendering Shadow Map Cascade " + std::to_wstring(cascadeIdx)));

		pPipelineManager->RasterizerStage.CurrentState.ViewPorts.SetState(0,CascadeViewPorts[cascadeIdx]);
		pPipelineManager->ApplyPipelineResources();
		//SetViewport(CascadeViewPorts[cascadeIdx]);
		//ConfigureViewports(pPipelineManager);
		//pPipelineManager->ApplyPipelineResources();

		XMVECTOR frustumCornersWS[8] =
		{
			XMVectorSet(-1.0f,1.0f,0.0f,1.0f),
			XMVectorSet(1.0f,1.0f,0.0f,1.0f),
			XMVectorSet(1.0f,-1.0f,0.0f,1.0f),
			XMVectorSet(-1.0f,-1.0f,0.0f,1.0f),
			XMVectorSet(-1.0f,1.0f,1.0f,1.0f),
			XMVectorSet(1.0f,1.0f,1.0f,1.0f),
			XMVectorSet(1.0f,-1.0f,1.0f,1.0f),
			XMVectorSet(-1.0f,-1.0f,1.0f,1.0f),
		};

		float prevSplitDist = cascadeIdx == 0 ? 0.0f : CascadeSplits[cascadeIdx - 1] * ShadowDist;
		float splitDist = CascadeSplits[cascadeIdx] * ShadowDist;

		XMMATRIX camView = view;
		XMMATRIX camProj = proj;

		XMVECTOR det;
		XMMATRIX invViewProj = XMMatrixInverse(&det,camView*camProj);
		for(UINT i = 0; i < 8; ++i)
			frustumCornersWS[i] = XMVector3TransformCoord(frustumCornersWS[i],invViewProj);

		// Scale by the shadow view distance
		for(UINT i = 0; i < 4; ++i)
		{
			XMVECTOR cornerRay = XMVectorSubtract(frustumCornersWS[i + 4],frustumCornersWS[i]);
			XMVECTOR nearCornerRay = XMVectorScale(cornerRay,prevSplitDist);
			XMVECTOR farCornerRay = XMVectorScale(cornerRay,splitDist);
			frustumCornersWS[i + 4] = XMVectorAdd(frustumCornersWS[i],farCornerRay);
			frustumCornersWS[i] = XMVectorAdd(frustumCornersWS[i],nearCornerRay);
		}

		// Calculate the centroid of the view frustum
		XMVECTOR sphereCenterVec = XMVectorZero();
		for(UINT i = 0; i < 8; ++i)
			sphereCenterVec = XMVectorAdd(sphereCenterVec,frustumCornersWS[i]);
		sphereCenterVec = XMVectorScale(sphereCenterVec,1.0f / 8.0f);

		// Calculate the radius of a bounding sphere
		XMVECTOR sphereRadiusVec = XMVectorZero();
		for(UINT i = 0; i < 8; ++i)
		{
			XMVECTOR dist = XMVector3Length(XMVectorSubtract(frustumCornersWS[i],sphereCenterVec));
			sphereRadiusVec = XMVectorMax(sphereRadiusVec,dist);
		}

		sphereRadiusVec = XMVectorRound(sphereRadiusVec);
		const float sphereRadius = XMVectorGetX(sphereRadiusVec);
		const float backupDist = sphereRadius + NearClip + Backup;


		XMVECTOR shadowCameraPosVec = sphereCenterVec;
		XMVECTOR backupDirVec = XMLoadFloat3(&lightDir);
		backupDirVec = XMVectorScale(backupDirVec,backupDist);
		shadowCameraPosVec = XMVectorAdd(shadowCameraPosVec,backupDirVec);

		XMFLOAT3 sphereCenter,shadowCameraPos;
		XMStoreFloat3(&sphereCenter,sphereCenterVec);
		XMStoreFloat3(&shadowCameraPos,shadowCameraPosVec);
		XMVECTOR up = XMVectorSet(0.0f,1.0f,0.0f,0.0f);

		XMMATRIX shadowCamView = XMMatrixIdentity();
		XMMATRIX shadowCamProj = XMMatrixOrthographicOffCenterLH(-sphereRadius,sphereRadius,-sphereRadius,sphereRadius,NearClip,backupDist + sphereRadius);
		XMMATRIX shadowCamViewProj = shadowCamView*shadowCamProj;
		shadowCamView = XMMatrixLookAtLH(shadowCameraPosVec,sphereCenterVec,up);
		shadowCamViewProj = shadowCamView*shadowCamProj;

		XMMATRIX shadowMatrix = shadowCamView*shadowCamProj;
		XMVECTOR shadowOrigin = XMVectorSet(0.0f,0.0f,0.0f,1.0f);
		shadowOrigin = XMVector4Transform(shadowOrigin,shadowMatrix);
		shadowOrigin = XMVectorScale(shadowOrigin,sMapSize / 2.0f);

		XMVECTOR roundedOrigin = XMVectorRound(shadowOrigin);
		XMVECTOR roundOffset = XMVectorSubtract(roundedOrigin,shadowOrigin);
		roundOffset = XMVectorScale(roundOffset,2.0f / sMapSize);
		roundOffset = XMVectorSetZ(roundOffset,0.0f);
		roundOffset = XMVectorSetW(roundOffset,0.0f);

		XMMATRIX shadowProj = shadowCamProj;
		shadowProj.r[3] = XMVectorAdd(shadowProj.r[3],roundOffset);
		shadowCamProj = shadowProj;
		shadowCamViewProj = shadowCamView*shadowCamProj;
		shadowMatrix = shadowCamView*shadowCamProj;

		RenderDepth(pPipelineManager,pParamManager,shadowCamView,shadowCamProj);

		const float bias = Bias;
		XMMATRIX texScaleBias;
		texScaleBias.r[0] = XMVectorSet(0.5f,0.0f,0.0f,0.0f);
		texScaleBias.r[1] = XMVectorSet(0.0f,-0.5f,0.0f,0.0f);
		texScaleBias.r[2] = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
		texScaleBias.r[3] = XMVectorSet(0.5f,0.5f,-bias,1.0f);
		shadowMatrix = XMMatrixMultiply(shadowMatrix,texScaleBias);

		XMFLOAT4 offset(Offsets[cascadeIdx].x,Offsets[cascadeIdx].y,0.0f,1.0);
		XMMATRIX cascadeOffsetMatrix = XMMatrixScaling(0.5f,0.5f,1.0f);
		cascadeOffsetMatrix.r[3] = XMLoadFloat4(&offset);
		shadowMatrix = XMMatrixMultiply(shadowMatrix,cascadeOffsetMatrix);

		XMFLOAT4X4 shadowMatrix4xx;
		XMStoreFloat4x4(&shadowMatrix4xx,shadowMatrix);
		cascadeMatrixArray4x4.push_back(shadowMatrix4xx);
		csdMatrixArray4x4[cascadeIdx] = shadowMatrix4xx;
		cascadeMatrixArray.push_back(shadowMatrix);

		float fNear,fFar;
		fNear = m_pScene->GetCamera()->m_fNear;
		fFar = m_pScene->GetCamera()->m_fFar;
		const float clipDist = fFar - fNear;

		float split = fNear + splitDist*clipDist;
		if(cascadeIdx == 0)
			cSplit.x = split;
		if(cascadeIdx == 1)
			cSplit.y = split;
		if(cascadeIdx == 2)
			cSplit.z = split;
		if(cascadeIdx == 3)
			cSplit.w = split;



		pPipelineManager->EndEvent();
	}
	for(int i=0; i < cascadeMatrixArray.size(); ++i)
	{

		if(i == 0)
			pParamManager->SetMatrixParameter(ShadowMatrix,&cascadeMatrixArray[i]);
		if(i == 1)
			pParamManager->SetMatrixParameter(ShadowMatrix2,&cascadeMatrixArray[i]);
		if(i == 2)
			pParamManager->SetMatrixParameter(ShadowMatrix3,&cascadeMatrixArray[i]);
		if(i == 3)
			pParamManager->SetMatrixParameter(ShadowMatrix4,&cascadeMatrixArray[i]);
	}
	/*pParamManager->SetMatrixParameter(ShadowMatrix,&matrixArray);*/
	pParamManager->SetMatrixArrayParameter(ShadowMatrices,4,csdMatrixArray4x4);
	XMVECTOR vSplit = XMLoadFloat4(&cSplit);
	pParamManager->SetVectorParameter(CascadeSplitsParameter,&vSplit);

	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,prevRenderTarget);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(prevDepthStencil);
	pPipelineManager->ApplyRenderTargets();


	pPipelineManager->RasterizerStage.CurrentState.ViewPorts.SetState(0,prevViewport);
	pPipelineManager->RasterizerStage.CurrentState.RasterizerState.SetState(prevRSS);
	pPipelineManager->ApplyPipelineResources();


	pPipelineManager->EndEvent();
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::RenderSceneNormally(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->BeginEvent(std::wstring(L"Normal Pass"));

	XMMATRIX m = m_pScene->GetCamera()->GetBody()->Transform.WorldMatrix();

	XMVECTOR scale;
	XMVECTOR translation;
	XMVECTOR rotation;
	XMMatrixDecompose(&scale,&rotation,&translation,m);


	XMMATRIX view = m_pScene->GetCamera()->GetBody()->Transform.GetView();
	XMMATRIX proj = m_pScene->GetCamera()->m_ProjMatrix;



	DoFrustumTests(view,proj);
	if(numSuccessfulTests == 0)
		return;


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
	XMStoreFloat3(&lightDir,lightDirV);

	
	XMMATRIX View = view;
	XMMATRIX Proj = proj;

	/*
	pParamManager->SetWorldMatrix(std::wstring(L"gWorld"),&world);
	pParamManager->SetViewMatrix(std::wstring(L"gView"),&View);
	pParamManager->SetProjectionMatrix(std::wstring(L"gProj"),&proj);*/


	/*pParamManager->SetWorldMatrix(&(world));*/
	pParamManager->SetViewMatrix(&(View));
	pParamManager->SetProjectionMatrix(&(proj));

	pParamManager->SetVectorParameter(CamPosWSParameter,&translation);


	if(frustumTests[0])
	{

		pParamManager->SetShaderResourceParameter(ShadowMapSrvParameter,ShadowMap);
		m_pScene->GetRoot()->Render(pPipelineManager,pParamManager,VT_PERSPECTIVE);

	}

	pPipelineManager->PixelShaderStage.CurrentState.ShaderResourceViews.SetState(0,NULL);
	pPipelineManager->PixelShaderStage.CurrentState.ShaderResourceViews.SetState(1,NULL);
	pPipelineManager->PixelShaderStage.CurrentState.ShaderResourceViews.SetState(2,NULL);
	pPipelineManager->ApplyPipelineResources();
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::SetRenderParams(IParameterManager* pParamManager)
{

}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::SetUsageParams(IParameterManager* pParamManager)
{


}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::SetScene(Scene* pScene)
{
	// Perform the root setting call for this view.
	m_pScene = pScene;

	//Traverse the actors who has a material

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
				AddActor(Entity);

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
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::SetEntity(ArkEntity3D* pEntity)
{
	// Perform the entity setting call for this view.
	m_pEntity = pEntity;
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::SetViewMatrix(const XMMATRIX& matrix)
{
	// Perform the view matrix setting for this view.
	RenderTask::SetViewMatrix(matrix);
}
////--------------------------------------------------------------------------------
void ViewCascadedShadowMap::AddActor(ArkEntity3D* pActor)
{
	for(int i=0; i < m_vActors.size() ; ++i)
	{
		if(m_vActors[i] == pActor)
		{
			return;
		}
	}
	m_vActors.push_back(pActor);
	ExecutorPtr pGeometry = (pActor->Visual.GetGeometry());
	PipelineExecutor11* p = pGeometry.get();

	ArkGeometry11* g = reinterpret_cast<ArkGeometry11*>(p);
	


	ComputeBoundingSpheres(ArkRenderer11::Get()->pPipeline,g,pActor->Transform.WorldMatrix());
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::DoFrustumTests(const XMMATRIX& view,const XMMATRIX& proj)
{
	frustumTests.clear();
	numSuccessfulTests = 0;

	ArkFrustum frustum;
	const XMMATRIX& vp = XMMatrixMultiply(view,proj);
	ArkMath::ComputeFrustum(vp,frustum);

	for(UINT i = 0; i < boundingSpheres.size(); ++i)
	{
		const ArkSphere3& sphere = boundingSpheres[i];
		UINT test = ArkMath::TestFrustumSphere(frustum,sphere);
		frustumTests.push_back(test);
		numSuccessfulTests += test;
	}
}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::VisualizeCascades(PipelineManager* pP,IParameterManager* param)
{

}
//--------------------------------------------------------------------------------
void ViewCascadedShadowMap::ComputeBoundingSpheres(PipelineManager* pPipelineManager,ArkGeometry11* pGeometry,const XMMATRIX& World)
{

	ResourcePtr stagingVB;
	ResourcePtr stagingIB;

	ArkBuffer11Config BufferConfig;
	BufferConfig.SetBindFlags(0);
	BufferConfig.SetByteWidth(pGeometry->GetVertexCount() * pGeometry->GetVertexSize()) ;
	BufferConfig.SetCPUAccessFlags(D3D11_CPU_ACCESS_READ);
	BufferConfig.SetMiscFlags(0);
	BufferConfig.SetStructureByteStride(0);
	BufferConfig.SetUsage(D3D11_USAGE_STAGING);

	stagingVB = ArkRenderer11::Get()->CreateVertexBuffer(&BufferConfig,0);
	BufferConfig.SetByteWidth(pGeometry->GetIndexCount() * 4);
	stagingIB = ArkRenderer11::Get()->CreateIndexBuffer(&BufferConfig,0);

	pPipelineManager->CopyResource(stagingVB,pGeometry->m_VB);
	pPipelineManager->CopyResource(stagingIB,pGeometry->m_IB);

	D3D11_MAPPED_SUBRESOURCE mapped;
	mapped = pPipelineManager->MapResource(stagingVB,0,D3D11_MAP_READ,0);
	const BYTE* verts = reinterpret_cast<const BYTE*>(mapped.pData);
	UINT stride = pGeometry->GetVertexSize();

	mapped = pPipelineManager->MapResource(stagingIB,0,D3D11_MAP_READ,0);
	const UINT* indices32 = reinterpret_cast<const UINT*>(mapped.pData);


	std::vector<XMFLOAT3> points;
	UINT indexCount = pGeometry->GetIndexCount();

	for(int i=0; i < indexCount; ++i)
	{
		UINT index = indices32[i];
		XMFLOAT3 point = *reinterpret_cast<const XMFLOAT3*>(verts + (index * stride));
		XMVECTOR position = XMLoadFloat3(&point);
		position = XMVector3TransformCoord(position,World);
		XMStoreFloat3(&point,position);
		points.push_back(point);
	}
	ArkSphere3 sphere = ArkMath::ComputeBoundingSphereFromPoints(&points[0],static_cast<UINT>(points.size()),sizeof(XMFLOAT3));
	boundingSpheres.push_back(sphere);

	pPipelineManager->UnMapResource(stagingVB,0);
	pPipelineManager->UnMapResource(stagingIB,0);
}
//--------------------------------------------------------------------------------
std::wstring ViewCascadedShadowMap::GetName()
{
	return(L"ViewShadowMap");
}
//--------------------------------------------------------------------------------
