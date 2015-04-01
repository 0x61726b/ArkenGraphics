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
	m_pRoot = new ArkNode3D();	
	m_pRoot->SetName(L"SceneRoot");
}
//--------------------------------------------------------------------------------
Scene::~Scene()
{
	// Delete all the actors that have been added to the scene.
	for ( auto pActor : m_vActors ) {
		Safe_Delete( pActor );
	}

	delete m_pRoot;
}
//--------------------------------------------------------------------------------
void Scene::Update(float dt)
{
	m_pRoot->Update(dt);
}
//--------------------------------------------------------------------------------
void Scene::PreRender(ArkRenderer11* pRenderer,VIEWTYPE type)
{
	m_pRoot->PreRender( pRenderer, type );
}
//--------------------------------------------------------------------------------
void Scene::Render(ArkRenderer11* pRenderer)
{
	for( auto camera : m_vCameras )
	{
		Parameters.InitRenderParams();
		camera->RenderFrame( pRenderer );
	}
}
//--------------------------------------------------------------------------------
void Scene::AddActor(Actor* pActor)
{
	m_pRoot->AttachChild( pActor->GetNode() );
	m_vActors.push_back(pActor);
}
//--------------------------------------------------------------------------------
void Scene::RemoveActor(Actor* pActor)
{
// First we iterate through the list of actors and remove any instances of 
	// this actor. 

	auto it = m_vActors.begin();

	while ( it != m_vActors.end() ) {
		if ( *it == pActor ) 
			it = m_vActors.erase( it );
		else
			it++;
	}

	// Now we remove the actor's node from the scene's root node, if it is there.
	// In general, an actor should always be attached directly to the scene's 
	// root, but there may be cases where an actor is attached to another actor.
	// To accommodate both situations, we do the removal by accessing the actor's
	// node's parent.  This ensures that the actor is completely removed from the
	// scene, and can safely be added back into the scene at a later time.

	ArkNode3D* pParent = static_cast<ArkNode3D*>( pActor->GetNode()->GetParent() );
	if ( pParent ) pParent->DetachChild( pActor->GetNode() );
}
//--------------------------------------------------------------------------------
void Scene::AddCamera( Camera* pCamera )
{
	m_vCameras.push_back( pCamera );
	pCamera->SetScene(this);
	AddActor(pCamera);
}
//--------------------------------------------------------------------------------
ArkNode3D* Scene:: GetRoot()
{
	return m_pRoot;
}