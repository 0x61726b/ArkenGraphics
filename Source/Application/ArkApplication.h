//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkApplication.h
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef __ArkApplication_h__
#define __ArkApplication_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkLog.h"

#include "ArkTimer.h"

#include "ArkConfigFile.h"
#include "DxDebugHelper.h"
#include "EventManager.h"
#include "IEventListener.h"
#include "IWndProc.h"

#include "EKeyDown.h"
#include "EKeyUp.h"
#include "EWindowResize.h"
#include "EMouseMove.h"
#include "EMouseLeave.h"
#include "EMouseRButtonDown.h"
#include "EMouseRButtonUp.h"


#include "Scene.h"
//--------------------------------------------------------------------------------

namespace Arkeng
{
	class ArkApplication : public IEventListener ,public IWndProc
	{
	public:
		ArkApplication();
		virtual ~ArkApplication();

		static ArkApplication* Get();

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

		ArkTimer* m_pTimer;

		bool m_bLoop;
	protected:
		Scene*				m_pScene;
	private:
		static ArkApplication* m_pApplication;

		ArkConfigFile*			m_cConfig;
		

	};
}


#endif // __Application_h__