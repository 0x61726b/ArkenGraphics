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
	for(auto pActor : m_vActors) {

		Safe_Delete(pActor);
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
	m_pRoot->PreRender(pRenderer,type);
}
//--------------------------------------------------------------------------------
void Scene::Render(ArkRenderer11* pRenderer)
{
	for(auto camera : m_vCameras)
	{
		Parameters.InitRenderParams();
		camera->RenderFrame(pRenderer);
	}
}
//--------------------------------------------------------------------------------
void Scene::AddActor(Actor* pActor)
{
	m_pRoot->AttachChild(pActor->GetNode());
	m_vActors.push_back(pActor);
}
//--------------------------------------------------------------------------------
void Scene::RemoveActor(Actor* pActor)
{
	// First we iterate through the list of actors and remove any instances of 
	// this actor. 

	auto it = m_vActors.begin();

	while(it != m_vActors.end()) {
		if(*it == pActor)
			it = m_vActors.erase(it);
		else
			it++;
	}

	// Now we remove the actor's node from the scene's root node, if it is there.
	// In general, an actor should always be attached directly to the scene's 
	// root, but there may be cases where an actor is attached to another actor.
	// To accommodate both situations, we do the removal by accessing the actor's
	// node's parent.  This ensures that the actor is completely removed from the
	// scene, and can safely be added back into the scene at a later time.

	ArkNode3D* pParent = static_cast<ArkNode3D*>(pActor->GetNode()->GetParent());
	if(pParent) pParent->DetachChild(pActor->GetNode());
}
//--------------------------------------------------------------------------------
void Scene::CalculateSceneBoundaries()
{
	ArkNode3D* Root = GetRoot();
	std::vector<ArkNode3D*> Nodes = Root->Nodes();
	std::vector<ArkGeometry11*> Geometries;

	for(int i=0; i < Nodes.size(); ++i)
	{
		ArkNode3D* ActorNodes = Nodes[i];
		std::vector<ArkEntity3D*> Leafs = ActorNodes->Leafs();
		for(int j=0; j < Leafs.size(); ++j)
		{
			ArkEntity3D* Entity = Leafs[j];

			if(Entity->Visual.Executor != nullptr)
			{
				ExecutorPtr pGeometry = (Entity->Visual.GetGeometry());
				PipelineExecutor11* p = pGeometry.get();

				ArkGeometry11* g = reinterpret_cast<ArkGeometry11*>(p);
				Geometries.push_back(g);
			}

		}
	}
	for( int i=0; i < Geometries.size(); ++i )
	{
		ArkGeometry11* pGeometry = Geometries[i];
		ArkBox boundingBox = pGeometry->GetBoundingBox();
		ActorBoundaries.push_back(boundingBox);
	}
	ArkBox bb;
	bb.Center() = XMVectorSet(0,0,0,0);
	bb.Extents() = XMVectorSet(10,10,10,0);
	SceneBoundary = bb;

}
//--------------------------------------------------------------------------------
void Scene::AddCamera(Camera* pCamera)
{
	m_vCameras.push_back(pCamera);
	pCamera->SetScene(this);
	AddActor(pCamera);
}
//--------------------------------------------------------------------------------
void Scene::AddLight(ArkLight* pLight)
{
	m_vLights.push_back(pLight);

	AddActor(pLight);
}
//--------------------------------------------------------------------------------
ArkLight* Scene::GetLight(unsigned int index)
{
	assert(index < m_vLights.size());

	return(m_vLights[index]);
}
//--------------------------------------------------------------------------------
unsigned int Scene::GetLightCount()
{
	return(m_vLights.size());
}
//--------------------------------------------------------------------------------
ArkNode3D* Scene::GetRoot()
{
	return m_pRoot;
}
//--------------------------------------------------------------------------------
Camera* Scene::GetCamera()
{
	return m_vCameras[0];
}
//--------------------------------------------------------------------------------
std::vector<Actor*> Scene::GetActors()
{
	return m_vActors;
}
//--------------------------------------------------------------------------------