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
#include "Camera.h"
#include "ArkSpriteRenderer11.h"

//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewShadowMap : public RenderTask
	{
	public:
		ViewShadowMap(ArkRenderer11& Renderer,ResourcePtr RenderTarget,ResourcePtr DepthTarget = 0);
		virtual ~ViewShadowMap();

		virtual void Update(float fTime);
		virtual void QueuePreTasks(ArkRenderer11* pRenderer);
		virtual void ExecuteTask(PipelineManager* pPipelineManager,IParameterManager* pParamManager);
		virtual void Resize(UINT width,UINT height);

		virtual void SetRenderParams(IParameterManager* pParamManager);
		virtual void SetUsageParams(IParameterManager* pParamManager);

		virtual void InitializeResources();

		virtual void SetViewMatrix(const XMMATRIX& matrix);
		//virtual void SetProjMatrix(const XMMATRIX& matrix);

		virtual void SetEntity(ArkEntity3D* pEntity);
		virtual void SetScene(Scene* pScene);

		void VisualizeCascades(PipelineManager* pP,IParameterManager* param);
		void ComputeBoundingSpheres(PipelineManager*,ArkGeometry11* pGeometry,const XMMATRIX& World);
		void DoFrustumTests( const XMMATRIX& view,const XMMATRIX& proj );
		void AddActor( ArkEntity3D* );
		std::vector<ArkEntity3D*> m_vActors;

		virtual std::wstring GetName();

		void RenderSceneNormally(PipelineManager* pP,IParameterManager* param);
		void RenderShadowMap(PipelineManager* pP,IParameterManager* param);
		void RenderDepth(PipelineManager* pP,IParameterManager* param,const XMMATRIX& view,const XMMATRIX& proj);

		static const UINT NumCascades = 4;

		std::vector<int> CascadeViewPorts;
	protected:
		int ResolutionX;
		int ResolutionY;

		int iBlendDisabledState;
		int iColorWriteDisabledBSS;
		int iBackFaceCullRS;
		int iDepthWriteEnabledDSS;
		int iAnisoSampler;
		int iShadowMapSampler;

		GeometryPtr sphereGeometry;
		std::shared_ptr<ArkRenderEffect11> VisualizeEffect;

		ArkSpriteRenderer11 m_pSpriteRenderer;

		XMFLOAT3 lightDir;

		ResourcePtr m_pRenderTarget;
		ResourcePtr m_pDepthTarget;
		std::shared_ptr<ArkMatrixParameter11>		WorldMatrixParameter;
		std::shared_ptr<ArkMatrixParameter11>		ViewMatrixParameter;
		std::shared_ptr<ArkMatrixParameter11>		WvPMatrixParameter;
		std::shared_ptr<ArkMatrixParameter11>		ShadowMatrix;
		std::shared_ptr<ArkMatrixParameter11>		ShadowMatrix2;
		std::shared_ptr<ArkMatrixParameter11>		ShadowMatrix3;
		std::shared_ptr<ArkMatrixParameter11>		ShadowMatrix4;
		std::shared_ptr<ArkMatrixArrayParameter11>		ShadowMatrices;
		std::shared_ptr<ArkVectorParameter11>       CascadeSplitsParameter;
		std::shared_ptr<ArkSamplerParameter11>		AnisoSamplerParameter;
		std::shared_ptr<ArkSamplerParameter11>		ShadowMapSamplerParameter;
		
		std::shared_ptr<ArkShaderResourceParameter11> ShadowMapSrvParameter;
		std::shared_ptr<ArkVectorParameter11>		LightColorParameter;
		std::shared_ptr<ArkVectorParameter11>		LightDirWSParameter;
		std::shared_ptr<ArkVectorParameter11>		CamPosWSParameter;
		//Depth Stencil Buffer
		ResourcePtr ShadowMap;

		ResourcePtr BackBuffer;
		ResourcePtr DepthTarget;

		
		std::shared_ptr<ArkRenderEffect11> pDepthEffect;
		std::shared_ptr<ArkRenderEffect11> pMainEffect;

		GeometryPtr pMesh;
		GeometryPtr pPlane;

		std::vector<ArkSphere3> boundingSpheres;
		std::vector<UINT> frustumTests;
		UINT numSuccessfulTests;


	};
};
//--------------------------------------------------------------------------------
#endif // ViewOcclusion_h
