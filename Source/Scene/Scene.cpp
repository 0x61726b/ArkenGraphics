//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Scene.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Scene.h"

//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Scene::Scene()
{
}
//--------------------------------------------------------------------------------
Scene::~Scene()
{
}
//--------------------------------------------------------------------------------
void Scene::Update(float dt)
{
}
//--------------------------------------------------------------------------------
void Scene::PreRender(ArkRenderer11* pRenderer)
{
}
//--------------------------------------------------------------------------------
void Scene::Render(ArkRenderer11* pRenderer)
{
	for( auto camera : m_vCameras )
	{
		camera->RenderFrame( pRenderer );
	}
}
//--------------------------------------------------------------------------------
void Scene::AddActor(Actor* pActor)
{
	m_vActors.push_back(pActor);
}
//--------------------------------------------------------------------------------
void Scene::RemoveActor(Actor* pActor)
{
	auto it = m_vActors.begin();

	while(it != m_vActors.end()) {
		if(*it == pActor)
			it = m_vActors.erase(it);
		else
			it++;
	}
}
//--------------------------------------------------------------------------------
void Scene::AddCamera( Camera* pCamera )
{
	m_vCameras.push_back( pCamera );
	pCamera->SetScene(this);
	AddActor(pCamera);
}
