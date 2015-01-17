//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Pch.h
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef __Application_h__
#define __Application_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "EventManager.h"
#include "IEventListener.h"
#include "IWndProc.h"

#include "EKeyDown.h"
//--------------------------------------------------------------------------------

namespace Arkeng
{
	class Application : public IEventListener ,public IWndProc
	{
	public:
		Application();
		virtual ~Application();

		static Application* Get();

		//Engine Functions
		virtual bool ConfigureEngineComponents() = 0;
		virtual void ShutdownEngineComponents() = 0;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
		virtual void MessageLoop();
		//~

		//IWndProc declaration
		virtual LRESULT WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
		virtual void BeforeRegisterWindowClass(WNDCLASSEX &wc);
		//~

		virtual bool HandleEvent(EventPtr pEvent);

		void RequestExit();
		EventManager EvtManager;

		bool m_bLoop;

	private:
		static Application* m_pApplication;
	};
}


#endif // __Application_h__