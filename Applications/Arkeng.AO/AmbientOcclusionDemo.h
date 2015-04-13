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

class AmbientOcclusionDemo : public ArkRenderApplication11
{
public:
	AmbientOcclusionDemo();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	bool ConfigureRenderingSetup();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();

	void HandleWindowResize( HWND handle,UINT width,UINT height );

	void UpdateParameters();
protected:

	Actor*									m_pActor;
	std::shared_ptr<ArkVectorParameter11>	m_pTimeFactors;
	std::shared_ptr<ArkVectorParameter11>   m_pFinalColor;
	std::shared_ptr<ArkVectorParameter11>   m_pLightColor;
	std::shared_ptr<ArkVectorParameter11>   m_pDiffuseColor;
	std::shared_ptr<ArkVectorParameterWriter11>	m_pDiffuseColorWriter;
	std::shared_ptr<ArkVectorParameter11>   m_pLightPosition;

	std::shared_ptr<ArkVectorParameter11>	m_pResolution;
	std::shared_ptr<ArkVectorParameter11>   m_pPerspective;
	std::shared_ptr<ArkVectorParameter11>	m_pFrustum;


};