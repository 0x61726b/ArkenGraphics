//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//App.h
//--------------------------------------------------------------------------------

#include "ArkRenderApplication11.h"
#include "Win32RenderWindow.h"
#include "D3D11Renderer.h"

using namespace Arkeng;

class App : public ArkRenderApplication11
{
public:
	App();
	~App();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();

	
};