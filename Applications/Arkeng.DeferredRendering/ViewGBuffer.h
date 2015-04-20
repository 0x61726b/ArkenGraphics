//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewGBuffer.h
//--------------------------------------------------------------------------------
#ifndef __ViewGBuffer_h__
#define __ViewGBuffer_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ViewGBuffer : public RenderTask
	{
	public:
		ViewGBuffer( ArkRenderer11& Renderer );
		virtual ~ViewGBuffer();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

        void SetTargets( std::vector<ResourcePtr>& GBufferTargets, ResourcePtr DepthTarget,
                          int Viewport );

		virtual std::wstring GetName();

	protected:

        ArkRenderer11&				m_Renderer;
		std::vector<ResourcePtr>	m_GBufferTargets;
		ResourcePtr					m_DepthTarget;
	};
}
//--------------------------------------------------------------------------------
#endif // ViewGBuffer_h