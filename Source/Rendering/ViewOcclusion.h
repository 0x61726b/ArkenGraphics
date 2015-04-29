//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewOcclusion.h
//--------------------------------------------------------------------------------
#ifndef __ViewOcclusion_h__
#define __ViewOcclusion_h__
//--------------------------------------------------------------------------------
#include "TaskCore.h"
#include "Actor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewOcclusion : public TaskCore
	{
	public:
		ViewOcclusion( ArkRenderer11& Renderer, ResourcePtr OcclusionTarget, ResourcePtr BlurTarget, ResourcePtr DepthNormalTarget );
		virtual ~ViewOcclusion();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		virtual std::wstring GetName( );

		

	protected:
		int ResolutionX;
		int ResolutionY;

		ResourcePtr DepthNormalBuffer;
		ResourcePtr OcclusionBuffer;
		ResourcePtr BilateralBuffer;

		ArkRenderEffect11*	pOcclusionEffect;
		ArkRenderEffect11*	pBilateralXEffect;
		ArkRenderEffect11*	pBilateralYEffect;

		std::shared_ptr<ArkShaderResourceParameter11> m_pDepthNormalBuffer;
		std::shared_ptr<ArkShaderResourceParameter11> m_pAmbientOcclusionBuffer;
		std::shared_ptr<ArkUnorderedAccessParameter11> m_pAmbientOcclusionTarget;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewOcclusion_h
