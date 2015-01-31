//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//D3D11RenderApplication.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "D3D11RenderApplication.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11Texture2DConfig.h"
#include "EventManager.h"
#include "EKeyDown.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
D3D11RenderApplication::D3D11RenderApplication()
{
	m_pWindow = 0;
	m_iWidth = 800;
	m_iHeight = 600;

	RequestEvent(WINDOW_RESIZE);
	RequestEvent(SYSTEM_KEYBOARD_KEYDOWN);
}
//--------------------------------------------------------------------------------
D3D11RenderApplication::~D3D11RenderApplication()
{
}
Win32RenderWindow* D3D11RenderApplication::CreateRenderWindow()
{
	return(new Win32RenderWindow());
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::ConfigureRenderingEngineComponents(UINT width,UINT height,D3D_FEATURE_LEVEL desiredLevel,D3D_DRIVER_TYPE driverType)
{
	// Create the renderer and initialize it for the desired device
	// type and feature level.
	m_pRenderer = new D3D11Renderer();

	if(!m_pRenderer->Initialize(driverType,desiredLevel))
	{
		MessageBox(0,L"Can't create hardware device.",L"Arkengine",MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		RequestExit();
		return false;
	}

	m_iWidth = width;
	m_iHeight = height;

	m_pWindow = CreateRenderWindow();
	m_pWindow->SetPosition(20,20);
	m_pWindow->SetSize(m_iWidth,m_iHeight);
	m_pWindow->SetCaption(GetName());
	m_pWindow->Initialize(this);

	Dx11SwapChainConfig Config;
	Config.SetWidth(m_pWindow->GetWidth());
	Config.SetHeight(m_pWindow->GetHeight());
	Config.SetOutputWindow(m_pWindow->GetHandle());


	m_pWindow->SetSwapChain(m_pRenderer->CreateSwapChain(&Config));


	//TODO: Pipeline Manager
	//TODO: Adding more things to Renderer class




	return true;
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::ConfigureRenderingSetup()
{
	m_pRenderer->pPipeline->ClearRenderTargets();

	ResourcePtr pRenderTarget = m_pRenderer->GetSwapChainResource(m_pWindow->GetSwapChain());
	Dx11Texture2DConfig DepthConfig;
	DepthConfig.SetWidth(m_iWidth);
	DepthConfig.SetHeight(m_iHeight);


	m_pRenderer->pPipeline->OutputMergerStage.State.RenderTargetViews.push_back(pRenderTarget->m_iResourceRTV);



	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport,sizeof(D3D11_VIEWPORT));

	viewport.Width = static_cast<float>(m_iWidth);
	viewport.Height = static_cast<float>(m_iHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	m_pRenderer->GetContext()->RSSetViewports(1,&viewport);

	m_pRenderer->pPipeline->ApplyRenderTargets();
	return true;
}
//--------------------------------------------------------------------------------
void D3D11RenderApplication::ShutdownRenderingEngineComponents()
{
	if(m_pRenderer) m_pRenderer->Shutdown();
	Safe_Delete(m_pRenderer);

	if(m_pWindow) m_pWindow->Shutdown();
	Safe_Delete(m_pWindow);
}
//--------------------------------------------------------------------------------
void D3D11RenderApplication::ShutdownRenderingSetup()
{
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e == WINDOW_RESIZE)
	{
		EWindowResizePtr pResize = std::static_pointer_cast<EWindowResize>(pEvent);

		this->HandleWindowResize(pResize->GetWindowHandle(),pResize->NewWidth(),pResize->NewHeight());

		return(true);

	}
	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return Application::HandleEvent(pEvent);
}
void D3D11RenderApplication::HandleWindowResize(HWND handle,UINT width,UINT height)
{
	if(width < 1) width = 1;
	if(height < 1) height = 1;
	m_iWidth = width;
	m_iHeight = height;

	if((m_pWindow != 0) && (m_pWindow->GetHandle() == handle))
	{
		m_pWindow->ResizeWindow(width,height);
		m_pRenderer->ResizeSwapChain(m_pWindow->GetSwapChain(),width,height);
	}
}