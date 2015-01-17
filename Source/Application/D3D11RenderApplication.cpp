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
#include "EventManager.h"
#include "EKeyDown.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
D3D11RenderApplication::D3D11RenderApplication()
{
	m_pWindow = 0;
	m_iWidth = 800;
	m_iHeight = 600;

	RequestEvent(SYSTEM_KEYBOARD_KEYDOWN);
}
//--------------------------------------------------------------------------------
D3D11RenderApplication::~D3D11RenderApplication()
{
}
Win32RenderWindow* D3D11RenderApplication::CreateRenderWindow( )
{
	return( new Win32RenderWindow() );
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::ConfigureRenderingEngineComponents( UINT width, UINT height, D3D_FEATURE_LEVEL desiredLevel, D3D_DRIVER_TYPE driverType )
{
	// Create the renderer and initialize it for the desired device
	// type and feature level.

	m_iWidth = width;
	m_iHeight = height;

	m_pWindow = CreateRenderWindow();
	m_pWindow->SetPosition( 20, 20 );
	m_pWindow->SetSize( m_iWidth, m_iHeight );
	m_pWindow->SetCaption( GetName() );
	m_pWindow->Initialize( this );

	return true;
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::ConfigureRenderingSetup()
{
	return true;
}
//--------------------------------------------------------------------------------
void D3D11RenderApplication::ShutdownRenderingEngineComponents()
{
}
//--------------------------------------------------------------------------------
void D3D11RenderApplication::ShutdownRenderingSetup()
{
}
//--------------------------------------------------------------------------------
bool D3D11RenderApplication::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
		unsigned int key = pKeyDown->GetCharacterCode();


	}
	return true;
}
void D3D11RenderApplication::HandleWindowResize(HWND handle,UINT width,UINT height)
{
	if(width < 1) width = 1;
	if(height < 1) height = 1;
	m_iWidth = width;
	m_iHeight = height;

	if((m_pWindow != 0) && (m_pWindow->GetHandle() == handle)) {
		m_pWindow->ResizeWindow(width,height);
	}
}