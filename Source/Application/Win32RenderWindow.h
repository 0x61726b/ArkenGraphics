//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Win32RenderWindow.h
//--------------------------------------------------------------------------------
#ifndef __Win32RenderWindow_h__
#define __Win32RenderWindow_h__
//--------------------------------------------------------------------------------
#include "RenderWindow.h"
#include "IWndProc.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Win32RenderWindow : public RenderWindow
	{
	public:
		Win32RenderWindow();
		virtual ~Win32RenderWindow();

		virtual void Initialize( IWndProc* );
		virtual void Shutdown();
		virtual void Paint();

		DirectX::XMVECTOR GetCursorPosition();
	};
};





//--------------------------------------------------------------------------------
#endif