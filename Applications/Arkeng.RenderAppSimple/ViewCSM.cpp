//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewCSM.h"
#include "PipelineManager.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11Texture2D.h"
#include "Scene.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewCSM::ViewCSM(ArkRenderer11& Renderer,ResourcePtr RenderTarget,ResourcePtr DepthTarget)
{
	ViewMatrix = DirectX::XMMatrixIdentity();
	ProjMatrix = DirectX::XMMatrixIdentity();
	SetRenderTargets( RenderTarget,DepthTarget );
	m_pShadowView = new ViewShadowMap(Renderer,m_pRenderTarget);
}
//--------------------------------------------------------------------------------
ViewCSM::~ViewCSM()
{
}
//--------------------------------------------------------------------------------
void ViewCSM::SetRenderTargets(ResourcePtr RenderTarget,ResourcePtr DepthTarget)
{
	m_pRenderTarget = RenderTarget;

	if ( nullptr != m_pDepthTarget ) 
	{
		ArkRenderer11::Get()->DeleteResource( m_pDepthTarget );
		m_pDepthTarget = nullptr;
	}

	if(m_pRenderTarget != nullptr)
	{
		std::shared_ptr<Dx11Resource> pResource = ArkRenderer11::Get()->GetResourceByIndex(m_pRenderTarget->m_iResource);


		if(pResource->GetType() == RT_TEXTURE2D)
		{
			std::shared_ptr<Dx11Texture2D> pTexture = std::dynamic_pointer_cast<Dx11Texture2D>(pResource);
			D3D11_TEXTURE2D_DESC desc = pTexture->GetActualDescription();
			if ( DepthTarget != NULL )
			{
				m_pDepthTarget = DepthTarget;
			} 
			else
			{
				Dx11Texture2DConfig DepthConfig;
				DepthConfig.SetDepthBuffer( desc.Width, desc.Height );
				m_pDepthTarget = ArkRenderer11::Get()->CreateTexture2D( &DepthConfig, 0 );			
			}
			D3D11_VIEWPORT viewport;
			viewport.Width = static_cast<float>(desc.Width);
			viewport.Height = static_cast<float>(desc.Height);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			SetViewport( ArkRenderer11::Get()->CreateViewport(viewport) );
			
		}

	}

	
}
//--------------------------------------------------------------------------------
void ViewCSM::Update(float dt)
{
}
//--------------------------------------------------------------------------------
void ViewCSM::QueuePreTasks(ArkRenderer11* pRenderer)
{
	if ( m_pEntity != NULL )
	{
		XMMATRIX view = ( m_pEntity->Transform.GetView());
		SetViewMatrix( view );
	}

	pRenderer->QueueTask(this);

	if( m_pScene )
	{
		m_pScene->PreRender( pRenderer,VT_PERSPECTIVE);
	}
	m_pShadowView->QueuePreTasks(pRenderer);
}
//--------------------------------------------------------------------------------
void ViewCSM::ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager)
{
	pPipelineManager->ClearRenderTargets();
	pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState(0,m_pRenderTarget->m_iResourceRTV);
	pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState(m_pDepthTarget->m_iResourceDSV);
	pPipelineManager->ApplyRenderTargets();

	float color[4] ={0,0,0,0};
	pPipelineManager->ClearBuffers(color,1.0f);

	ConfigureViewports(pPipelineManager);


	pPipelineManager->ClearPipelineResources();

}
//--------------------------------------------------------------------------------
void ViewCSM::Resize(UINT width,UINT height)
{
	ArkRenderer11::Get()->ResizeTexture( m_pDepthTarget,width,height );
	ArkRenderer11::Get()->ResizeViewport(m_iViewports[0],width,height);
}
//--------------------------------------------------------------------------------
std::wstring ViewCSM::GetName()
{
	return std::wstring(L"View CSM");
}
//--------------------------------------------------------------------------------
void ViewCSM::SetScene(Scene* scene)
{
	m_pShadowView->SetScene(scene);
}
//--------------------------------------------------------------------------------
void ViewCSM::SetEntity(ArkEntity3D* Entity)
{
	m_pShadowView->SetEntity(Entity);
}
//--------------------------------------------------------------------------------
