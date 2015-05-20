//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------
#ifndef __ViewOmnidirectionalShadowMap_h__
#define __ViewOmnidirectionalShadowMap_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
#include "ViewSettings.h"
#include "ArkVectorParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkShaderResourceParameter11.h"
#include "ArkSamplerParameter11.h"
#include "ViewCubicMap.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ViewOmnidirectionalShadowMap : public RenderTask
	{
	public:
		ViewOmnidirectionalShadowMap (ArkRenderer11& Renderer, const ViewSettings&);
		virtual ~ViewOmnidirectionalShadowMap ();

		//TaskCore Interface
		virtual void Update( float dt );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager,IParameterManager* pParamManager );
		//~

		//RenderTask Interface
		virtual void Resize( UINT width,UINT height );
		virtual void SetScene(Scene* pScene);
		//~

		void RenderDepthOnly(PipelineManager* pPipelineManager,IParameterManager* pParamManager);
		void RenderNormally(PipelineManager* pPipelineManager,IParameterManager* pParamManager);
		virtual std::wstring GetName();

		static const UINT ShadowMapSize = 1024;
	protected:
		ViewCubicMap* m_pViewCubicMap;
		ResourcePtr m_pRenderTarget;
		ResourcePtr m_pDepthTarget;

		int				m_iShadowMapViewport;
		int				m_iViewport;
		ResourcePtr		ShadowMap;

		ResourcePtr		BackBuffer;
		ResourcePtr		DepthBuffer;

		XMVECTOR		m_vLightPosition;
		XMMATRIX		m_mLightProjMatrix;
		XMMATRIX		m_mLightViewMatrices[6];
		XMFLOAT4X4*		m_mLightView4x4;
		std::shared_ptr<ArkVectorParameter11>			LightPosWSParameter;
		std::shared_ptr<ArkVectorParameter11>			LightColorParameter;
		std::shared_ptr<ArkVectorParameter11>			ViewPositionWS;
		std::shared_ptr<ArkVectorParameter11>			ViewDirectionWS;
		std::shared_ptr<ArkVectorParameter11>			LightRangeParameter;
		std::shared_ptr<ArkMatrixArrayParameter11>		LightViewProjMatricesParameter;
		std::shared_ptr<ArkMatrixParameter11>			WorldInverseTranspose;
		std::shared_ptr<ArkShaderResourceParameter11>   ShadowMapCubeParameter;
		std::shared_ptr<ArkSamplerParameter11>			ShadowSamplerParameter;
		int				ShadowMapSampler;

	};
};



//--------------------------------------------------------------------------------
#endif

