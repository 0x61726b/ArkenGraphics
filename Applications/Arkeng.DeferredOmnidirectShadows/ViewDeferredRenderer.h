//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDeferredRenderer.h
//--------------------------------------------------------------------------------
#ifndef __ViewDeferredRenderer_h__
#define __ViewDeferredRenderer_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ViewGBuffer.h"
#include "ViewLights.h"
#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
#include "ViewDepthNormal.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewDeferredRenderer : public RenderTask
	{
	public:
		ViewDeferredRenderer( ArkRenderer11& Renderer, ResourcePtr RenderTarget );
		virtual ~ViewDeferredRenderer();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		virtual void SetEntity( ArkEntity3D* pEntity );
		virtual void SetScene( Scene* pScene );

		virtual void SetViewMatrix( const XMMATRIX& matrix );
		virtual void SetProjMatrix( const XMMATRIX& matrix );
		
		void SetClipPlanes( float NearClip, float FarClip );
		void SetupViews();

		virtual std::wstring GetName();

	protected:

        int							m_iCurrentViewport;
		int							ResolutionX;
		int							ResolutionY;

        float						m_fNearClip;
        float						m_fFarClip;

		std::vector<ResourcePtr>	m_GBuffer[GBufferOptMode::NumSettings][AAMode::NumSettings];
		ResourcePtr					m_DepthTarget[AAMode::NumSettings];
		ResourcePtr					m_ReadOnlyDepthTarget[AAMode::NumSettings];
		ResourcePtr					m_FinalTarget[AAMode::NumSettings];
		int							m_iViewport[AAMode::NumSettings];
		ResourcePtr					m_ResolveTarget;
		ResourcePtr					m_BackBuffer;
		ResourcePtr					m_ShadowMap;

		ViewGBuffer*				m_pGBufferView;
		ViewLights*					m_pLightsView;
		ViewDepthNormal*			m_pDepthView;

		ArkSpriteRenderer11			m_SpriteRenderer;
		SpriteFontPtr				m_pFont;

		XMFLOAT3					InterpLightPos;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewDeferredRenderer_h
