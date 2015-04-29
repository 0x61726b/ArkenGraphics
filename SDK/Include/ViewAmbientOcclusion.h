//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ViewAmbientOcclusion
//
// This class extends the standard perspective view to produce a depth/normal 
// buffer, then produce an occlusion buffer from it, and then finally produce an
// end image like a perspective view would.
//--------------------------------------------------------------------------------
#ifndef ViewAmbientOcclusion_h
#define ViewAmbientOcclusion_h
//--------------------------------------------------------------------------------
#include "PerspectiveView.h"
#include "ViewOcclusion.h"
#include "ViewDepthNormal.h"
#include "Actor.h"
#include "ArkShaderResourceParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ViewAmbientOcclusion : public PerspectiveView
	{
	public:
		ViewAmbientOcclusion( ArkRenderer11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget = 0 );
		virtual ~ViewAmbientOcclusion();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetEntity( ArkEntity3D* pEntity );
		virtual void SetScene( Scene* pScene );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		virtual void SetViewMatrix( const XMMATRIX& matrix );
		virtual void SetProjMatrix( const XMMATRIX& matrix );

		void SetVisualizationActive( bool active );

		virtual std::wstring GetName();

		

	protected:

		int ResolutionX;
		int ResolutionY;

		ResourcePtr DepthNormalBuffer;
		ResourcePtr OcclusionBuffer;
		ResourcePtr BilateralBuffer;

		ViewOcclusion*		pOcclusionView;
		ViewDepthNormal*	pDepthNormalView;

		Actor*				pVisActor;

		std::shared_ptr<ArkShaderResourceParameter11>	pDepthBufferParameter;
		std::shared_ptr<ArkShaderResourceParameter11>	pOcclusionBufferParameter;

		bool				bRenderVisualization;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewAmbientOcclusion_h
