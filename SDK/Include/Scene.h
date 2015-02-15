//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Scene.h
//--------------------------------------------------------------------------------
#ifndef __Scene_h__
#define __Scene_h__
//--------------------------------------------------------------------------------
#include "Actor.h"
#include "Camera.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Update( float dt );
		virtual void PreRender( ArkRenderer11* Renderer );
		virtual void Render( ArkRenderer11* Renderer );

		void AddActor( Actor* actor );
		void RemoveActor(Actor* actor);

		void AddCamera( Camera* pCamera );
		Camera* GetCamera();


	protected:
		std::vector< Actor* >		m_vActors;
		std::vector< Camera*>		m_vCameras;
	};
};

//--------------------------------------------------------------------------------
#endif