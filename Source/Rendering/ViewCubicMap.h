//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#ifndef __ViewCubicMap_h__
#define __ViewCubicMap_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkShaderResourceParameter11.h"
#include "ViewSettings.h"
#include "ArkVectorParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkShaderResourceParameter11.h"
#include "ArkSamplerParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewCubicMap : public RenderTask
	{
	public:
		ViewCubicMap(ArkRenderer11& Renderer,const ViewSettings& Settings);
		virtual ~ViewCubicMap();

		virtual void QueuePreTasks(ArkRenderer11* pRenderer);
		virtual void ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager);

		virtual void SetUsageParams(IParameterManager* pParamManager);

		virtual std::wstring GetName();
		virtual void Update(float dt);
		virtual void Resize(UINT,UINT);
		virtual void SetScene(Scene* pScene);

		static const UINT SMSize = 1024;

		void SetupLights();
	protected:

		ArkRenderEffect11*		m_pDepthEffect;
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
#endif // ViewDepthNormal_h
