//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewDepthNormal.h
//--------------------------------------------------------------------------------
#ifndef __ViewShadows_h__
#define __ViewShadows_h__
//--------------------------------------------------------------------------------
#include "PerspectiveView.h"
#include "ArkShaderResourceParameter11.h"
#include "ViewShadowMap.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewShadows : public PerspectiveView
	{
	public:
		ViewShadows( ArkRenderer11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget );
		virtual ~ViewShadows();

		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );

		virtual void SetUsageParams( IParameterManager* pParamManager );
		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual std::wstring GetName( );

		/*void SetLightPosition( XMFLOAT3 pos );*/

		int ResolutionX,ResolutionY;
	protected:
		ResourcePtr DepthBuffer;
		ViewShadowMap* pShadowMapView;

		std::shared_ptr<ArkShaderResourceParameter11>	pDepthBufferParameter;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewDepthNormal_h
