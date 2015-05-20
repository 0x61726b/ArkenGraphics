//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewWireFrame.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewWireFrame.h"
#include "PipelineManager.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11Texture2D.h"
#include "Scene.h"
#include "ViewSettings.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11RasterizerStateConfig.h"
#include "ArkGeometryGenerator11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewWireframe::ViewWireframe(ArkRenderer11& Renderer,const ViewSettings& Settings)
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

	Dx11RasterizerStateConfig RS;
	RS.FillMode = D3D11_FILL_WIREFRAME;

	iWireFrameRS =
		Renderer.CreateRasterizerState(&RS);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(desc.Width);
	viewport.Height = static_cast<float>(desc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	SetViewport(ArkRenderer11::Get()->CreateViewport(viewport));

	m_pEffect = std::make_shared<ArkRenderEffect11>();

	m_pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")
		));
	m_pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")
		));

	
}
//--------------------------------------------------------------------------------
ViewWireframe::~ViewWireframe()
{
}
//--------------------------------------------------------------------------------
void ViewWireframe::SetScene(Scene* p)
{
	m_pScene = p;
}
//--------------------------------------------------------------------------------
void ViewWireframe::SetBoxes(Scene* p)
{
	m_CubeGeometry = GeometryPtr(new ArkGeometry11());

	
	ArkBox sceneBB = p->SceneBoundary;
	m_BoundingBoxes.push_back(sceneBB);
	
	float w,h,d;
	XMFLOAT3 c;
	XMStoreFloat3( &c, sceneBB.Center() );
	XMFLOAT3 e;
	XMStoreFloat3( &e, sceneBB.Extents() );
	XMFLOAT3 diff;
	XMStoreFloat3( &diff, sceneBB.Extents() - sceneBB.Center());

	ArkGeometryGenerator11::GenerateCube(m_CubeGeometry,e.x,e.y,e.z);
	m_CubeGeometry->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CubeGeometry->LoadToBuffers();
	m_Cubes.push_back(m_CubeGeometry);
}
//--------------------------------------------------------------------------------
void ViewWireframe::Update(float dt)
{
}
//--------------------------------------------------------------------------------
void ViewWireframe::QueuePreTasks(ArkRenderer11* pRenderer)
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
}
//--------------------------------------------------------------------------------
void ViewWireframe::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,BackBuffer->m_iResourceRTV);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(DepthBuffer->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={vColor.x,vColor.y,vColor.z,vColor.w};
	pPipelineManager->ClearBuffers(color,1.0f);

	ConfigureViewports(pPipelineManager);

	SetRenderParams(pParamManager);

	pPipelineManager->ClearPipelineResources();

	m_pEffect->m_iRasterizerState = iWireFrameRS;

	for(int i=0; i < m_BoundingBoxes.size(); ++i)
	{
		pPipelineManager->Draw(*m_pEffect,m_Cubes[i],pParamManager);
	}
}
//--------------------------------------------------------------------------------
void ViewWireframe::Resize(UINT width,UINT height)
{
	//ArkRenderer11::Get()->ResizeTexture( m_pDepthTarget,width,height );
	//ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
//--------------------------------------------------------------------------------
std::wstring ViewWireframe::GetName()
{
	return std::wstring(L"ViewWireframe");
}
