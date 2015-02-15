//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Camera.h
//--------------------------------------------------------------------------------
#ifndef __Camera_h__
#define __Camera_h__
//--------------------------------------------------------------------------------
#include "Actor.h"
#include "RenderTask.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Scene;

	class Camera : public Actor
	{
	public:
		Camera();
		virtual ~Camera();


		void SetCameraView( RenderTask* );

		RenderTask*		GetCameraView();

		void RenderFrame( ArkRenderer11* Renderer );

		void SetScene( Scene* pScene );

	protected:
		RenderTask*			m_vCameraView;
		Scene*				m_pScene;
	};
};



//--------------------------------------------------------------------------------
#endif
