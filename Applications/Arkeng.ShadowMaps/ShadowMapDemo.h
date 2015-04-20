//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//AmbientOcclusionDemo.h
//--------------------------------------------------------------------------------
#include "ArkRenderApplication11.h"
#include "Win32RenderWindow.h"
#include "ArkRenderer11.h"

#include "ArkVectorParameter11.h"
#include "ArkVectorParameterWriter11.h"
#include "Camera.h"
#include "Scene.h"

#include "ArkSpriteRenderer11.h"
#include "ArkFontLoader.h"


using namespace Arkeng;

class ShadowMapDemo : public ArkRenderApplication11
{
public:
	ShadowMapDemo();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	bool ConfigureRenderingSetup();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();

	void HandleWindowResize( HWND handle,UINT width,UINT height );
protected:

	Actor*									m_pActor;
	Actor*									m_pPlaneActor;

	std::shared_ptr<ArkVectorParameter11>   m_pLightPosition;

};