//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//D3D11RenderApplication.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderApplication11.h"
#include "Dx11SwapChainConfig.h"
#include "Dx11Texture2DConfig.h"

#include "EventManager.h"
#include "EKeyDown.h"
#include "EKeyUp.h"
#include "EMouseRButtonDown.h"
#include "EMouseRButtonUp.h"
#include "EMouseMove.h"
#include "EMouseLeave.h"
#include "EFrameStart.h"

#include "PipelineManager.h"
#include "ArkFirstPersonCamera.h"
#include "IParameterManager.h"
#include "PerspectiveView.h"



//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkRenderApplication11::ArkRenderApplication11()
{
	m_pWindow = 0;
	m_iWidth = 800;
	m_iHeight = 600;

	m_pCamera = 0;
	m_pRenderView = 0;
	m_pRenderer = 0;

	RequestEvent( WINDOW_RESIZE );
	RequestEvent( RENDER_FRAME_START );
    RequestEvent( SYSTEM_RBUTTON_DOWN );
    RequestEvent( SYSTEM_RBUTTON_UP );
    RequestEvent( SYSTEM_MOUSE_MOVE );
    RequestEvent( SYSTEM_MOUSE_LEAVE );
}
//--------------------------------------------------------------------------------
ArkRenderApplication11::~ArkRenderApplication11()
{
}
Win32RenderWindow* ArkRenderApplication11::CreateRenderWindow()
{
	return(new Win32RenderWindow());
}
//--------------------------------------------------------------------------------
bool ArkRenderApplication11::ConfigureRenderingEngineComponents(UINT width,UINT height,D3D_FEATURE_LEVEL desiredLevel,D3D_DRIVER_TYPE driverType)
{
	// Create the renderer and initialize it for the desired device
	// type and feature level.
	m_pRenderer = new ArkRenderer11();

	ArkLog::Get( LogType::Renderer ).Output(L"Test");
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


	m_pBackBuffer = m_pRenderer->GetSwapChainResource( m_pWindow->GetSwapChain() );
	//TODO: Pipeline Manager
	//TODO: Adding more things to Renderer class




	return true;
}
//--------------------------------------------------------------------------------
bool ArkRenderApplication11::ConfigureRenderingSetup()
{

	PerspectiveView* pPerspView = new PerspectiveView( *m_pRenderer,m_pBackBuffer );
	m_pRenderView = pPerspView;

	m_pCamera = new ArkFirstPersonCamera();
	m_pCamera->SetEventManager( &CameraEventHub );
	XMVECTOR rot = XMVectorSet(0,0,0,0);
	m_pCamera->GetNode()->Transform.Rotation() = XMMatrixRotationRollPitchYawFromVector(rot);
	m_pCamera->GetNode()->Transform.Position() = XMVectorSet( 0.0f, 10.0f, -20.0f,0.0f );
	m_pCamera->SetCameraView( m_pRenderView );
	m_pCamera->SetProjectionParams(0.1f,1000.0f, static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight), static_cast<float>(3.14f) / 2.0f);

	m_pScene->AddCamera( m_pCamera );
	return true;
}
//--------------------------------------------------------------------------------
void ArkRenderApplication11::ShutdownRenderingEngineComponents()
{
	if(m_pRenderer) m_pRenderer->Shutdown();
	Safe_Delete(m_pRenderer);

	if(m_pWindow) m_pWindow->Shutdown();
	Safe_Delete(m_pWindow);
}
//--------------------------------------------------------------------------------
void ArkRenderApplication11::ShutdownRenderingSetup()
{
	Safe_Delete( m_pScene );
}
//--------------------------------------------------------------------------------
bool ArkRenderApplication11::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e == WINDOW_RESIZE)
	{
		EWindowResizePtr pResize = std::static_pointer_cast<EWindowResize>(pEvent);

		this->HandleWindowResize(pResize->GetWindowHandle(),pResize->NewWidth(),pResize->NewHeight());

		return(true);

	}
	if(e == SYSTEM_KEYBOARD_KEYUP)
	{
		EKeyUpPtr pKeyDown = std::static_pointer_cast<EKeyUp>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return ArkApplication::HandleEvent(pEvent);
}
void ArkRenderApplication11::HandleWindowResize(HWND handle,UINT width,UINT height)
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
	// Update the projection matrix of our camera
	if ( m_pCamera != 0 ) {
		m_pCamera->SetAspectRatio( static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight) );
	}
	if( m_pRenderView != 0 )
	{
		m_pRenderView->Resize( width,height );
	}
}