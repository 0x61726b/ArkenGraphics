//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewDepthNormal.h"
#include "ArkEntity3D.h"
#include "Scene.h"
#include "Dx11Texture2DConfig.h"
#include "ArkLog.h"
#include "IParameterManager.h"
#include "PipelineManager.h"
#include "Dx11Texture2D.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewDepthNormal::ViewDepthNormal( ArkRenderer11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget )
	: PerspectiveView( Renderer, RenderTarget, DepthTarget )
{
	// This view is more or less the same as a ViewPerspective, with only a 
	// different scene rendering type (as indicated below with VT_LINEAR_DEPTH_NORMAL).  
	// In addition, the render target that gets passed to this view will serve as
	// the depth/normal buffer as opposed to a standard render target.

	m_pDepthNormalBuffer = Renderer.m_pParamMgr->GetShaderResourceParameterRef( std::wstring( L"DepthNormalBuffer" ) );
}
//--------------------------------------------------------------------------------
ViewDepthNormal::~ViewDepthNormal()
{
}
//--------------------------------------------------------------------------------
void ViewDepthNormal::QueuePreTasks( ArkRenderer11* pRenderer )
{
	if ( m_pEntity != NULL )
	{
		XMMATRIX view = m_pEntity->Transform.GetView();
		SetViewMatrix( view );
	}

	// Queue this view into the renderer for processing.
	pRenderer->QueueTask( this );

	if ( m_pScene )
	{
		// Run through the graph and pre-render the entities
		m_pScene->PreRender( pRenderer, VT_LINEAR_DEPTH_NORMAL );
	}
}
//--------------------------------------------------------------------------------
void ViewDepthNormal::ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager )
{
	if ( m_pScene )
	{
		// Set the parameters for rendering this view
		pPipelineManager->ClearRenderTargets();
		pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState( 0, m_pRenderTarget->m_iResourceRTV );
		pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState( m_pDepthTarget->m_iResourceDSV );
		pPipelineManager->ApplyRenderTargets();

		float color[4] = { vColor.x,vColor.y,vColor.z,vColor.w };
		pPipelineManager->ClearBuffers( color, 1.0f );

		// Configure the desired viewports in this pipeline
		ConfigureViewports( pPipelineManager );

		// Set this view's render parameters
		SetRenderParams( pParamManager );

		// Run through the graph and render each of the entities
		m_pScene->GetRoot()->Render( pPipelineManager, pParamManager, VT_LINEAR_DEPTH_NORMAL );
	}
}
//--------------------------------------------------------------------------------
void ViewDepthNormal::SetUsageParams( IParameterManager* pParamManager )
{
	// This view will bind the depth/normal buffer to the "DepthNormalBuffer" shader
	// resource view parameter, so that other views can make use of it.

	pParamManager->SetShaderResourceParameter( m_pDepthNormalBuffer, m_pRenderTarget );
}
//--------------------------------------------------------------------------------
std::wstring ViewDepthNormal::GetName()
{
	return( L"ViewDepthNormal" );
}
//--------------------------------------------------------------------------------
