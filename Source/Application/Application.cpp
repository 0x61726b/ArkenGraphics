//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Application.cpp
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "Application.h"

using namespace Arkeng;

Application* Application::m_pApplication = NULL;
//--------------------------------------------------------------------------------
Application::Application():
m_bLoop(true)
{
	m_pApplication = this;


	SetEventManager(&EvtManager);

	RequestEvent(SYSTEM_KEYBOARD_KEYDOWN);
}
//--------------------------------------------------------------------------------
Application::~Application() { }
//--------------------------------------------------------------------------------
Application* Application::Get()
{
	return m_pApplication;
}
//--------------------------------------------------------------------------------
void Application::RequestExit()
{
	PostQuitMessage(0);
}
//--------------------------------------------------------------------------------
bool Application::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e==SYSTEM_KEYBOARD_KEYDOWN)
	{
		EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);

		unsigned int key = pKeyDown->GetCharacterCode();

		if(key==VK_ESCAPE)
		{
			RequestExit();
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
void Application::MessageLoop()
{
	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
			{
				return;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Call the overloaded application update function.
		Update();
	}
}
//--------------------------------------------------------------------------------
void Application::BeforeRegisterWindowClass(WNDCLASSEX &wc)
{
	// This function is intended to be overriden in dervived classes
}
//--------------------------------------------------------------------------------
LRESULT Application::WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg)
	{

		case WM_CREATE:
		{
			// Automatically return 0 to allow the window to proceed in the
			// creation process.

			return(0);
		} break;

		case WM_PAINT:
		{
			// This message is handled by the default handler to avoid a 
			// repeated sending of the message.  This results in the ability
			// to process all pending messages at once without getting stuck
			// in an eternal loop.
		} break;

		case WM_CLOSE:
		{
			// This message is sent when a window or an application should
			// terminate.
		} break;

		case WM_DESTROY:
		{
			// This message is sent when a window has been destroyed.
			PostQuitMessage(0);
			return(0);
		} break;
		case WM_KEYDOWN:
		{
			EKeyDownPtr pEvent = EKeyDownPtr(new EKeyDown(hwnd,wparam,lparam));
			EvtManager.ProcessEvent(pEvent);
		} break;
	}
	return(DefWindowProc(hwnd,msg,wparam,lparam));
}
