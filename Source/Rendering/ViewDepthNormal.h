//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#ifndef __ViewDepthNormal_h__
#define __ViewDepthNormal_h__
//--------------------------------------------------------------------------------
#include "PerspectiveView.h"
#include "ArkShaderResourceParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Entity3D;

	class ViewDepthNormal : public PerspectiveView
	{
	public:
		ViewDepthNormal( ArkRenderer11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget );
		virtual ~ViewDepthNormal();

		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );

		virtual void SetUsageParams( IParameterManager* pParamManager );

		virtual std::wstring GetName( );

	protected:
		std::shared_ptr<ArkShaderResourceParameter11> m_pDepthNormalBuffer;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewDepthNormal_h
