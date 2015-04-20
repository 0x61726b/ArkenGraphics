//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewOcclusion.h
//--------------------------------------------------------------------------------
#ifndef __ViewShadowMap_h__
#define __ViewShadowMap_h__
//--------------------------------------------------------------------------------
#include "TaskCore.h"
#include "Actor.h"
#include "PerspectiveView.h"
#include "ViewDepthNormal.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewShadowMap : public PerspectiveView
	{
	public:
		ViewShadowMap( ArkRenderer11& Renderer, ResourcePtr RenderTarget,ResourcePtr DepthTarget = 0 );
		virtual ~ViewShadowMap();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		virtual void SetViewMatrix( const XMMATRIX& matrix );
		virtual void SetProjMatrix( const XMMATRIX& matrix );

		virtual void SetEntity( ArkEntity3D* pEntity );
		virtual void SetScene( Scene* pScene );

		void SetLightParameters( XMMATRIX& View,XMMATRIX& Proj, XMVECTOR& Pos);

		virtual std::wstring GetName( );

	protected:
		int ResolutionX;
		int ResolutionY;

		ResourcePtr DepthNormalBuffer;

		ViewDepthNormal* pDepthView;

		ArkRenderEffect11*	pShadowMapGenerationEffect;

		std::shared_ptr<ArkShaderResourceParameter11> m_pDepthNormalBuffer;

		XMMATRIX			LightViewMatrix;
		XMMATRIX			LightProjMatrix;

		std::shared_ptr<ArkMatrixParameter11> m_pLightViewMatrix;
		std::shared_ptr<ArkMatrixParameter11> m_pLightProjMatrix;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewOcclusion_h
