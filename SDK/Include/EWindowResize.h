//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EWindowResize.h
//--------------------------------------------------------------------------------
#ifndef __EWindowResize_h__
#define __EWindowResize_h__
//--------------------------------------------------------------------------------
#include "EWindow.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EWindowResize : public EWindow
	{
	public:
		EWindowResize(HWND hwnd,unsigned int wparam,long lparam);
		virtual ~EWindowResize();

		virtual std::wstring GetEventName();
		virtual eEvent GetEventType();

		int NewWidth();
		int NewHeight();
		bool IsMaxHide();	// Message is sent to all pop-up windows when some other window is maximized.
		bool IsMaximized();	// The window has been maximized.
		bool IsMaxShow();	// Message is sent to all pop-up windows when some other window has been restored to its former size.
		bool IsMinimized(); // The window has been minimized.
		bool IsRestored();	// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
	};
	typedef std::shared_ptr<EWindowResize> EWindowResizePtr;
};
//--------------------------------------------------------------------------------
#endif