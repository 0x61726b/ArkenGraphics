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
	class ArkEntity3D;
	class Scene;

	enum VIEWTYPE
	{
		VT_INVALID,
		VT_PERSPECTIVE,
		VT_LINEAR_DEPTH_NORMAL,
		VT_DUAL_PARABOLOID_ENVMAP,
		VT_GBUFFER,
		VT_LIGHTS,
        VT_FINALPASS,
		VT_NUM_VIEW_TYPES
	};    

	class RenderTask : public TaskCore
	{
	public:
		RenderTask();
		virtual ~RenderTask();

		virtual void Resize( UINT w, UINT h ) = 0;

		virtual void SetScene( Scene* pScene );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		void SetViewport(int viewport,unsigned int index=0);
		void SetViewportCount(unsigned int count);
		void ConfigureViewports( PipelineManager* pPipeline );

		const Dx11ViewPort& GetViewport( unsigned int index );

		virtual void SetViewMatrix( const DirectX::XMMATRIX& matrix );
		virtual void SetProjMatrix( const DirectX::XMMATRIX& matrix );
		const DirectX::XMMATRIX& GetViewMatrix( );
		const DirectX::XMMATRIX& GetProjMatrix( );

		virtual void SetEntity( ArkEntity3D* pEntity );

		virtual void SetBackColor( XMFLOAT4& c );
		
	protected:
		Scene*			m_pScene;
		ArkEntity3D*	m_pEntity;
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ProjMatrix;

		XMFLOAT4		  vColor;

		int				m_iViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
		unsigned int	m_uiViewportCount;
		
	};
};

//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------