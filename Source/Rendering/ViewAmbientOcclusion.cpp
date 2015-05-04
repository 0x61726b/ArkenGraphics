//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ViewAmbientOcclusion.h"
#include "ArkEntity3D.h"
#include "ArkNode3D.h"
#include "Dx11Texture2DConfig.h"
#include "ArkLog.h"
//#include "ActorGenerator.h"
#include "IParameterManager.h"
#include "DepthViewSettings.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewAmbientOcclusion::ViewAmbientOcclusion( ArkRenderer11& Renderer, const ViewSettings& Settings )
: PerspectiveView( Renderer,Settings )
{
	D3D11_TEXTURE2D_DESC desc = Settings.BackBuffer->m_pTexture2dConfig->GetTextureDesc();

	ResolutionX = desc.Width;
	ResolutionY = desc.Height;

	// Create the resources to be used in this rendering algorithm.  The 
	// depth/normal buffer will have four components, and be used as a render
	// target and shader resource.

	Dx11Texture2DConfig config;
	config.SetColorBuffer( ResolutionX, ResolutionY );
	config.SetBindFlags( D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET );
	
	DepthNormalBuffer = Renderer.CreateTexture2D( &config, 0 );
	
	// The occlusion buffer is calculated in the compute shader, so requires
	// an unordered access view as well as a shader resource.
	config.SetColorBuffer( ResolutionX, ResolutionY );
	config.SetFormat( DXGI_FORMAT_R32_FLOAT );
	config.SetBindFlags( D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS );
	OcclusionBuffer = Renderer.CreateTexture2D( &config, 0 );
	
	// The blurring buffer will be used to blur the input buffer, and uses the same
	// configuration as the occlusion buffer.

	BilateralBuffer = Renderer.CreateTexture2D( &config, 0 );

	// Create the two sub-views to perform the extra rendering operations for
	// ambient occlusion.

	pOcclusionView = new ViewOcclusion( Renderer, OcclusionBuffer, BilateralBuffer, DepthNormalBuffer );
	DepthViewSettings DepthViewS(DepthNormalBuffer,Settings.DepthBuffer);

	pDepthNormalView = new ViewDepthNormal( Renderer, DepthViewS );

	// Create the visualization actor and send in the occlusion buffer.

	/*pVisActor = 0;
	pVisActor = ActorGenerator::GenerateVisualizationTexture2D( Renderer, 
			DepthNormalBuffer, 0 );

	bRenderVisualization = false;*/

	// Grab references to the desired parameters in order to quickly set their
	// values as needed later on.

	pDepthBufferParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef( std::wstring( L"DepthNormalBuffer" ) );
	pOcclusionBufferParameter = Renderer.m_pParamMgr->GetShaderResourceParameterRef( std::wstring( L"AmbientOcclusionBuffer" ) );

}
//--------------------------------------------------------------------------------
ViewAmbientOcclusion::~ViewAmbientOcclusion()
{
	Safe_Delete( pOcclusionView );
	Safe_Delete( pDepthNormalView );

	
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::Update( float fTime )
{
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::QueuePreTasks( ArkRenderer11* pRenderer )
{
	// Call the super class's predraw in order to queue it in the renderer.  The
	// views are processed in a LIFO order, so this will be executed last in both
	// single- or multi-threaded mode.

	PerspectiveView::QueuePreTasks( pRenderer );

	// Next we call the predraw method of each of the supporting views.  Once 
	// again, the views are processed in reverse order of submission, so the depth
	// normal view will be processed first, then the occlusion view, then the 
	// actual standard perspective view (whose objects will use the ambient 
	// occlusion buffers).

	pOcclusionView->QueuePreTasks( pRenderer );
	pDepthNormalView->QueuePreTasks( pRenderer );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager )
{
	// Here we are simply calling our super class's draw method to perform the 
	// standard rendering process.

	PerspectiveView::ExecuteTask( pPipelineManager, pParamManager );


	// Add the visualization rendering into the scene

	/*if ( bRenderVisualization ) {
		pVisActor->GetNode()->Render( pPipelineManager, pParamManager, VT_PERSPECTIVE );
	}*/
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::Resize( UINT width, UINT height )
{
	ResolutionX = width;
	ResolutionY = height;

	ArkRenderer11::Get()->ResizeTexture( m_pDepthTarget, width, height );
	ArkRenderer11::Get()->ResizeTexture( DepthNormalBuffer, width, height );
	ArkRenderer11::Get()->ResizeTexture( OcclusionBuffer, width, height );
	ArkRenderer11::Get()->ResizeTexture( BilateralBuffer, width, height );
	ArkRenderer11::Get()->ResizeViewport( m_iViewports[0], width, height );

	pDepthNormalView->Resize( width, height );
	pOcclusionView->Resize( width, height );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetRenderParams( IParameterManager* pParamManager )
{
	// Set the parameters for this view to be able to perform its processing
	// sequence.  In this case, we set the depth/normal buffer and the ambient
	// occlusion buffer as shader resources in addition to the standard parameters
	// that are set for a normal perspective view.

	PerspectiveView::SetRenderParams( pParamManager );

	pParamManager->SetShaderResourceParameter( pDepthBufferParameter, DepthNormalBuffer );
	pParamManager->SetShaderResourceParameter( pOcclusionBufferParameter, OcclusionBuffer );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetUsageParams( IParameterManager* pParamManager )
{
	// Set the parameters for allowing an application to use the current resources
	// for rendering.

	pParamManager->SetShaderResourceParameter( pOcclusionBufferParameter, OcclusionBuffer );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetViewMatrix( const XMMATRIX& matrix )
{
	// Perform the view matrix setting for this view.
	RenderTask::SetViewMatrix( matrix );

	// Propagate the view matrix to the depth/normal view.
	pDepthNormalView->SetViewMatrix( matrix );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetProjMatrix( const XMMATRIX& matrix )
{
	// Perform the projection matrix setting for this view.
	RenderTask::SetProjMatrix( matrix );

	// Propagate the projection matrix to the depth/normal view.
	pDepthNormalView->SetProjMatrix( matrix );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetScene( Scene* pScene )
{
	// Perform the root setting call for this view.
	m_pScene = pScene;

	// Propagate the root setting call to the depth/normal view.
	pDepthNormalView->SetScene( pScene );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetEntity( ArkEntity3D* pEntity )
{
	// Perform the entity setting call for this view.
	m_pEntity = pEntity;

	// Propagate the entity call to the depth/normal view.
	pDepthNormalView->SetEntity( pEntity );
}
//--------------------------------------------------------------------------------
void ViewAmbientOcclusion::SetVisualizationActive( bool active )
{
	bRenderVisualization = active;
}
//--------------------------------------------------------------------------------
std::wstring ViewAmbientOcclusion::GetName()
{
	return( L"ViewAmbientOcclusion" );
}
//--------------------------------------------------------------------------------
