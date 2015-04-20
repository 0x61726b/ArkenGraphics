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
#include "ViewShadowMap.h"
#include "ArkEntity3D.h"
#include "ArkNode3D.h"
#include "Dx11Texture2DConfig.h"
#include "ArkLog.h"
//#include "ActorGenerator.h"
#include "IParameterManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ViewShadowMap::ViewShadowMap( ArkRenderer11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget)
	: PerspectiveView( Renderer, RenderTarget, DepthTarget ),LightProjMatrix( XMMatrixIdentity() ),LightViewMatrix( XMMatrixIdentity() )
{
	D3D11_TEXTURE2D_DESC desc = RenderTarget->m_pTexture2dConfig->GetTextureDesc();

	Dx11Texture2DConfig config;
	config.SetColorBuffer( desc.Width, desc.Height);
	config.SetBindFlags( D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET );
	DepthNormalBuffer = Renderer.CreateTexture2D( &config, 0 );

	pDepthView = new ViewDepthNormal( Renderer, DepthNormalBuffer, DepthTarget );

	m_pDepthNormalBuffer = Renderer.m_pParamMgr->GetShaderResourceParameterRef(std::wstring(L"DepthNormalBuffer"));
	m_pLightViewMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"lightViewMatrix"));
	m_pLightProjMatrix = Renderer.m_pParamMgr->GetMatrixParameterRef(std::wstring(L"lightProjectionMatrix"));

}
//--------------------------------------------------------------------------------
ViewShadowMap::~ViewShadowMap()
{
	Safe_Delete( pShadowMapGenerationEffect );
}
//--------------------------------------------------------------------------------
void ViewShadowMap::Resize( UINT width, UINT height )
{
	// The resources themselves will be resized by the parent view, but we just
	// need to record how big of a render target we will be processing.
	ResolutionX = width;
	ResolutionY = height;
}
//--------------------------------------------------------------------------------
void ViewShadowMap::Update( float fTime )
{
}
//--------------------------------------------------------------------------------
void ViewShadowMap::QueuePreTasks( ArkRenderer11* pRenderer )
{
	// Queue this view into the renderer for processing.
	PerspectiveView::QueuePreTasks( pRenderer );

	pDepthView->QueuePreTasks( pRenderer );

}
//--------------------------------------------------------------------------------
void ViewShadowMap::ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager )
{
	PerspectiveView::ExecuteTask( pPipelineManager,pParamManager );
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetRenderParams( IParameterManager* pParamManager )
{
	// Set the parameters for this view to be able to perform its processing
	// sequence.  In this case, we set the depth/normal buffer as a shader 
	// resource and the occlusion buffer as an unordered access view.

	PerspectiveView::SetRenderParams( pParamManager );

	pParamManager->SetShaderResourceParameter(  std::dynamic_pointer_cast<ArkRenderParameter11>(m_pDepthNormalBuffer), DepthNormalBuffer );

	pParamManager->SetMatrixParameter( m_pLightViewMatrix,&LightViewMatrix );
	pParamManager->SetMatrixParameter( m_pLightProjMatrix,&LightProjMatrix );
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetUsageParams( IParameterManager* pParamManager )
{
	// Set the parameters for allowing an application to use the current resources
	// for rendering.

	
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetScene( Scene* pScene )
{
	// Perform the root setting call for this view.
	m_pScene = pScene;

	// Propagate the root setting call to the depth/normal view.
	pDepthView->SetScene( pScene );
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetEntity( ArkEntity3D* pEntity )
{
	// Perform the entity setting call for this view.
	m_pEntity = pEntity;

	// Propagate the entity call to the depth/normal view.
	pDepthView->SetEntity( pEntity );
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetViewMatrix( const XMMATRIX& matrix )
{
	// Perform the view matrix setting for this view.
	RenderTask::SetViewMatrix( matrix );

	// Propagate the view matrix to the depth/normal view.
	pDepthView->SetViewMatrix( matrix );


}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetLightParameters( XMMATRIX& View,XMMATRIX& Proj, XMVECTOR& Pos)
{
	LightProjMatrix = Proj;
	LightViewMatrix = View;
}
//--------------------------------------------------------------------------------
void ViewShadowMap::SetProjMatrix( const XMMATRIX& matrix )
{
	// Perform the projection matrix setting for this view.
	RenderTask::SetProjMatrix( matrix );

	// Propagate the projection matrix to the depth/normal view.
	pDepthView->SetProjMatrix( matrix );

}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
std::wstring ViewShadowMap::GetName( )
{
	return( L"ViewShadowMap" );
}
//--------------------------------------------------------------------------------
