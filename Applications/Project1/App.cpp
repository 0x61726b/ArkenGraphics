//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//App.cpp
//--------------------------------------------------------------------------------
#include "App.h"
#include "Dx11RenderTargetView.h"
#include "EventManager.h"
#include "EKeyDown.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
App AppInstance;
//--------------------------------------------------------------------------------
App::App()
{

}
//--------------------------------------------------------------------------------
App::~App()
{
}
bool App::ConfigureEngineComponents()
{
	if(!ConfigureRenderingEngineComponents(1920,1080,D3D_FEATURE_LEVEL_11_0)) {
		return(false);
	}

	if(!ConfigureRenderingSetup()) {
		return(false);
	}

	return true;
}
//--------------------------------------------------------------------------------
void App::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}
//--------------------------------------------------------------------------------
void App::Initialize()
{
	
}
//--------------------------------------------------------------------------------
void App::Update()
{
	float color[4] = { 1,0,0,0 };
	m_pRenderer->pPipeline->ClearBuffers(color,1000.0f);

	m_pRenderer->Present( m_pWindow->GetHandle(),m_pWindow->GetSwapChain() );
}
//--------------------------------------------------------------------------------
void App::Shutdown()
{
}
//--------------------------------------------------------------------------------
bool App::HandleEvent(EventPtr e)
{
	return D3D11RenderApplication::HandleEvent( e );
}
//--------------------------------------------------------------------------------
std::wstring App::GetName()
{
	return L"Basic App";
}
//--------------------------------------------------------------------------------