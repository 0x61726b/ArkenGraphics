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


#include "ArkGeometry11.h"
#include "ArkMaterial11.h"
#include "Camera.h"
#include "Scene.h"

#include "AppSettings.h"

using namespace Arkeng;

class DeferredRenderingDemo : public ArkRenderApplication11
{
public:
	DeferredRenderingDemo();
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

	void DrawHUD();

    MaterialPtr		        m_pMaterial;
    std::shared_ptr<ArkRenderEffect11>      m_pGBufferEffect[GBufferOptMode::NumSettings];
    int					    m_iGBufferDSState;
    int                     m_iGBufferRSState;

    ResourcePtr             m_DiffuseTexture;
    ResourcePtr             m_NormalMap;

	Actor*					m_pActor;
};