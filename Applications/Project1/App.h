//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//App.h
//--------------------------------------------------------------------------------

#include "D3D11RenderApplication.h"
#include "Win32RenderWindow.h"

using namespace Arkeng;

class App : public D3D11RenderApplication 
{
public:
	App();

	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();
};