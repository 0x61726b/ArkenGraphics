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
#include "ViewGBuffer.h"
#include "ArkEntity3D.h"
#include "Scene.h"
#include "Dx11Texture2dConfig.h"
#include "ArkLog.h"
#include "IParameterManager.h"
#include "PipelineManager.h"
#include "Dx11Texture2d.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewGBuffer::ViewGBuffer( ArkRenderer11& Renderer )
    : m_Renderer( Renderer )
{
	 ViewMatrix =  XMMatrixIdentity() ;
	 ProjMatrix =  XMMatrixIdentity() ;
}
//--------------------------------------------------------------------------------
ViewGBuffer::~ViewGBuffer()
{
}
//--------------------------------------------------------------------------------
void ViewGBuffer::Update( float fTime )
{
}
//--------------------------------------------------------------------------------
void ViewGBuffer::QueuePreTasks( ArkRenderer11* pRenderer )
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
		m_pScene->GetRoot()->PreRender( pRenderer, VT_GBUFFER );
	}
}
//--------------------------------------------------------------------------------
void ViewGBuffer::ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager )
{
	if ( m_pScene )
	{
		// Set the parameters for rendering this view
		pPipelineManager->ClearRenderTargets();
		for ( unsigned int i = 0; i < m_GBufferTargets.size(); ++i )
			pPipelineManager->OutputMergerStage.CurrentState.RenderTargetViews.SetState( i, m_GBufferTargets[i]->m_iResourceRTV );
		pPipelineManager->OutputMergerStage.CurrentState.DepthTargetViews.SetState( m_DepthTarget->m_iResourceDSV );
		pPipelineManager->ApplyRenderTargets();

		// Configure the desired viewports in this pipeline
		ConfigureViewports( pPipelineManager );

		// Clear the G-Buffer targets
		
		pPipelineManager->ClearBuffers( new float[4] { 0,0,0,0 }, 1.0f, 0 );

		// Set this view's render parameters
		SetRenderParams( pParamManager );

		// Run through the graph and render each of the entities
		m_pScene->GetRoot()->Render( pPipelineManager, pParamManager, VT_GBUFFER );
	}
}
//--------------------------------------------------------------------------------
void ViewGBuffer::Resize( UINT width, UINT height )
{
	// Nothing needed here, since the main render view handles the resizing of
	// the resources and the viewports.
}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetRenderParams( IParameterManager* pParamManager )
{
	pParamManager->SetViewMatrix( &ViewMatrix );
	pParamManager->SetProjectionMatrix( &ProjMatrix );
}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetUsageParams( IParameterManager* pParamManager )
{

}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetTargets( std::vector<ResourcePtr>& GBufferTargets,
                                ResourcePtr DepthTarget, int Viewport )
{
    m_GBufferTargets = GBufferTargets;

    SetViewport( Viewport );
    m_DepthTarget = DepthTarget;
}
//--------------------------------------------------------------------------------
std::wstring ViewGBuffer::GetName()
{
	return( L"ViewGBuffer" );
}
//--------------------------------------------------------------------------------