//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#ifndef __ViewShadowMap_h__
#define __ViewShadowMap_h__
//--------------------------------------------------------------------------------
#include "TaskCore.h"
#include "Actor.h"
#include "Camera.h"
#include "ArkSpriteRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewShadowMap : public RenderTask
	{
	public:
		ViewShadowMap(ArkRenderer11& Renderer,ResourcePtr RenderTarget,ResourcePtr DepthTarget=0);
		virtual ~ViewShadowMap();

		virtual void Update(float fTime);
		virtual void QueuePreTasks(ArkRenderer11* pRenderer);
		virtual void ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager);
		virtual void Resize(UINT width,UINT height);
		virtual void SetUsageParams(IParameterManager* pParamManager);

		virtual std::wstring GetName();

		virtual void SetViewMatrix(const XMMATRIX& matrix);
		virtual void SetProjMatrix(const XMMATRIX& matrix);

		virtual void SetScene(Scene* Scene);

	protected:
		std::shared_ptr<ArkShaderResourceParameter11> m_pDepthNormalBuffer;

		int iBlendDisabledState;
		int iColorWriteDisabledBSS;
		int iBackFaceCullRS;
		int iDepthWriteEnabledDSS;
		int iAnisoSampler;
		int iShadowMapSampler;

		ResourcePtr ShadowMap;

		ResourcePtr BackBuffer;
		ResourcePtr DepthTarget;


		std::shared_ptr<ArkRenderEffect11> pDepthEffect;
		std::shared_ptr<ArkRenderEffect11> pMainEffect;

		std::shared_ptr<ArkSamplerParameter11>		AnisoSamplerParameter;
		std::shared_ptr<ArkSamplerParameter11>		ShadowMapSamplerParameter;

		std::shared_ptr<ArkShaderResourceParameter11> ShadowMapSrvParameter;
		std::shared_ptr<ArkVectorParameter11>		LightColorParameter;
		std::shared_ptr<ArkVectorParameter11>		LightDirWSParameter;
		std::shared_ptr<ArkVectorParameter11>		CamPosWSParameter;

		std::shared_ptr<ArkMatrixParameter11>		LightViewMatrix;
		std::shared_ptr<ArkMatrixParameter11>		LightProjMatrix;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewDepthNormal_h
