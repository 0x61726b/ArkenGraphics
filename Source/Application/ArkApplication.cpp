//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Application.cpp
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "ArkApplication.h"

using namespace Arkeng;

ArkApplication* ArkApplication::m_pApplication = NULL;
//--------------------------------------------------------------------------------
ArkApplication::ArkApplication():
m_bLoop(true)
{
	m_pApplication = this;


	SetEventManager(&EvtManager);

	RequestEvent(SYSTEM_KEYBOARD_KEYDOWN);
	RequestEvent(SYSTEM_KEYBOARD_KEYUP);

	m_cConfig = new ArkConfigFile();
	m_cConfig->Open();

	m_pTimer = new ArkTimer();
	m_pTimer->Update();

	m_pScene = new Scene();

	ArkLog* l = new ArkLog(LogType::Base);
	l->Open();
	l->Output(L"Log Started.");

	std::wstring& buildNumber = m_cConfig->GetConfig().BuildNumber;

	l->Output(L"Arkengine Build " + buildNumber);
}
//--------------------------------------------------------------------------------
ArkApplication::~ArkApplication()
{
	if(m_pScene != NULL)
	{
		delete m_pScene;
	}

	if(m_pTimer != NULL)
	{
		delete m_pTimer;
	}

	ArkLog::Get(LogType::Base).Close();
	delete m_cConfig;
}
//--------------------------------------------------------------------------------
ArkApplication* ArkApplication::Get()
{
	return m_pApplication;
}
//--------------------------------------------------------------------------------
void ArkApplication::RequestExit()
{
	/*_CrtDumpMemoryLeaks();*/
	PostQuitMessage(0);
}
//--------------------------------------------------------------------------------
bool ArkApplication::HandleEvent(EventPtr pEvent)
{
	eEvent e = pEvent->GetEventType();

	if(e==SYSTEM_KEYBOARD_KEYUP)
	{
		EKeyUpPtr pKeyDown = std::static_pointer_cast<EKeyUp>(pEvent);

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
void ArkApplication::MessageLoop()
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
void ArkApplication::BeforeRegisterWindowClass(WNDCLASSEX &wc)
{
	// This function is intended to be overriden in dervived classes
}
//--------------------------------------------------------------------------------
LRESULT ArkApplication::WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
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
	case WM_SIZE:
	{
		EWindowResizePtr pEvent = EWindowResizePtr(new EWindowResize(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
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
	case WM_KEYUP:
	{
		EKeyUpPtr pEvent = EKeyUpPtr(new EKeyUp(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
	} break;
	case WM_MOUSEMOVE:
	{
		EMouseMovePtr pEvent = EMouseMovePtr(new EMouseMove(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
	} break;
	case WM_MOUSELEAVE:
	{
		EMouseLeavePtr pEvent = EMouseLeavePtr(new EMouseLeave(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
	}break;
	case WM_RBUTTONDOWN:
	{
		EMouseRButtonDownPtr pEvent = EMouseRButtonDownPtr(new EMouseRButtonDown(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
	}break;
	case WM_RBUTTONUP:
	{
		EMouseRButtonUpPtr pEvent = EMouseRButtonUpPtr(new EMouseRButtonUp(hwnd,wparam,lparam));
		EvtManager.ProcessEvent(pEvent);
	}break;
	}
	return(DefWindowProc(hwnd,msg,wparam,lparam));
}
