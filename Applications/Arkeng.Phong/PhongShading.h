//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PhongShading.h
//--------------------------------------------------------------------------------
#include "ArkRenderApplication11.h"
#include "Win32RenderWindow.h"
#include "ArkRenderer11.h"

#include "ArkVectorParameter11.h"
#include "Camera.h"
#include "Scene.h"

#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"
using namespace Arkeng;

class PhongShading : public ArkRenderApplication11
{
public:
	PhongShading();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	bool ConfigureRenderingSetup();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();

protected:
	Actor* m_pActor;
	Actor*		m_pPlane;
	MaterialPtr m_pMaterial;
	ArkRenderEffect11	m_Effect;

	ArkLight*										m_pLight;
	std::shared_ptr<ArkVectorParameter11>			m_pLightPositionWriter;
	std::shared_ptr<ArkVectorParameter11>			m_pLightColor;

	ResourcePtr										m_pVertexBuffer;
	int												m_VertexLayout;
	ResourcePtr										m_pIndexBuffer;


};