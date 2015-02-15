//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderTask.h
//--------------------------------------------------------------------------------
#ifndef __RenderTask_h__
#define __RenderTask_h__
//--------------------------------------------------------------------------------
#include "TaskCore.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Scene;

	class RenderTask : public TaskCore
	{
	public:
		RenderTask();
		virtual ~RenderTask();

		virtual void Resize( UINT w, UINT h ) = 0;

		virtual void SetScene( Scene* pScene );

		void SetViewport(int viewport,unsigned int index=0);
		void SetViewportCount(unsigned int count);
		void ConfigureViewports( PipelineManager* pPipeline );

		const Dx11ViewPort& GetViewport( unsigned int index );

	protected:
		Scene*			m_pScene;

		
		int				m_iViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
		unsigned int	m_uiViewportCount;
		
	};
};

//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------