//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------

#include "ArkRenderApplication11.h"
#include "Win32RenderWindow.h"
#include "ArkRenderer11.h"

#include "Camera.h"
#include "Scene.h"

using namespace Arkeng;

class RenderAppSimple : public ArkRenderApplication11
{
public:
	RenderAppSimple();
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
	MaterialPtr m_pMaterial;
	ArkRenderEffect11	m_Effect;

	ResourcePtr             m_pVertexBuffer;
	int                     m_VertexLayout;
	ResourcePtr             m_pIndexBuffer;
};