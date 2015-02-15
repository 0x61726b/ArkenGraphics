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
	int resX = 0;
	int resY = 0;

	resX = ArkConfigFile::Get().GetConfig().DesiredResolutionX;
	resY = ArkConfigFile::Get().GetConfig().DesiredResolutionY;

	if(!ConfigureRenderingEngineComponents(resX,resY,D3D_FEATURE_LEVEL_11_0)) {
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
	m_pScene->Update( 1/60 );
	m_pScene->Render( m_pRenderer );

	m_pRenderer->Present( m_pWindow->GetHandle(),m_pWindow->GetSwapChain() );
}
//--------------------------------------------------------------------------------
void App::Shutdown()
{
}
//--------------------------------------------------------------------------------
bool App::HandleEvent(EventPtr e)
{
	return ArkRenderApplication11::HandleEvent( e );
}
//--------------------------------------------------------------------------------
std::wstring App::GetName()
{
	return L"Basic App";
}
//--------------------------------------------------------------------------------