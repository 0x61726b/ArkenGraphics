//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------
#ifndef __ViewCSM_h__
#define __ViewCSM_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
#include "ViewShadowMap.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ViewCSM : public RenderTask
	{
	public:
		ViewCSM(ArkRenderer11& Renderer, ResourcePtr RenderTarget,ResourcePtr DepthTarget = 0 );
		virtual ~ViewCSM();

		//TaskCore Interface
		virtual void Update( float dt );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager,IParameterManager* pParamManager );

		virtual void SetScene(Scene*);
		virtual void SetEntity( ArkEntity3D* pEntity );
		//~

		//RenderTask Interface
		virtual void Resize( UINT width,UINT height );
		//~

		void SetRenderTargets(ResourcePtr RenderTarget,ResourcePtr DepthTarget);

		virtual std::wstring GetName();
	protected:
		ResourcePtr m_pRenderTarget;
		ResourcePtr m_pDepthTarget;

		ViewShadowMap* m_pShadowView;
	};
};



//--------------------------------------------------------------------------------
#endif

