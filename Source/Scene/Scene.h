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
#include "ArkNode3D.h"
#include "Camera.h"
#include "ArkParameterContainer.h"
#include "ArkLight.h"
#include "ArkSphere3.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Update( float dt );
		virtual void PreRender( ArkRenderer11* Renderer,VIEWTYPE type );
		virtual void Render( ArkRenderer11* Renderer );

		void AddActor( Actor* actor );
		void RemoveActor(Actor* actor);

		void AddCamera( Camera* pCamera );
		Camera* GetCamera();

		void AddLight( ArkLight* pLight );
		ArkLight* GetLight( unsigned int index );
		unsigned int GetLightCount();

		ArkNode3D* GetRoot();

		std::vector<Actor*> GetActors();
		ArkSphere3 GetSceneBoundingSphere();
		ArkParameterContainer Parameters;
	protected:
		ArkNode3D* m_pRoot;
		std::vector< Actor* >		m_vActors;
		std::vector< Camera*>		m_vCameras;
		std::vector< ArkLight* >    m_vLights;

	};
};

//--------------------------------------------------------------------------------
#endif