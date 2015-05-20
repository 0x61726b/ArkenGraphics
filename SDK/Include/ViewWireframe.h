//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PerspectiveView.h
//--------------------------------------------------------------------------------
#ifndef __ViewWireframe_h__
#define __ViewWireframe_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
#include "ViewSettings.h"
#include "ArkBox.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ViewWireframe : public RenderTask
	{
	public:
		ViewWireframe(ArkRenderer11& Renderer, const ViewSettings&);
		virtual ~ViewWireframe();

		//TaskCore Interface
		virtual void Update( float dt );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager,IParameterManager* pParamManager );
		//~

		//RenderTask Interface
		virtual void Resize( UINT width,UINT height );
		//~
		void SetBoxes(Scene*);

		virtual void SetScene(Scene* p);

		virtual std::wstring GetName();
	protected:
		ResourcePtr BackBuffer;
		ResourcePtr DepthBuffer;

		std::vector<ArkBox>		m_BoundingBoxes;
		std::vector<GeometryPtr> m_Cubes;

		GeometryPtr		m_CubeGeometry;
		std::shared_ptr<ArkRenderEffect11>	m_pEffect;
		int iWireFrameRS;

	};
};



//--------------------------------------------------------------------------------
#endif

