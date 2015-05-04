//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------
#ifndef __PerspectiveView_h__
#define __PerspectiveView_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
#include "ViewSettings.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PerspectiveView : public RenderTask
	{
	public:
		PerspectiveView(ArkRenderer11& Renderer, const ViewSettings&);
		virtual ~PerspectiveView();

		//TaskCore Interface
		virtual void Update( float dt );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager,IParameterManager* pParamManager );
		//~

		//RenderTask Interface
		virtual void Resize( UINT width,UINT height );
		//~

		void SetRenderTargets(ResourcePtr RenderTarget,ResourcePtr DepthTarget);

		virtual std::wstring GetName();
	protected:
		ResourcePtr m_pRenderTarget;
		ResourcePtr m_pDepthTarget;

	};
};



//--------------------------------------------------------------------------------
#endif

