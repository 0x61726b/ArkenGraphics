//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EWindow.h
//--------------------------------------------------------------------------------
#ifndef __EWindow_h__
#define __EWindow_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IEvent.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EWindow : public IEvent
	{
	public:
		EWindow( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EWindow();

		HWND GetWindowHandle();
		unsigned int GetWParam();
		unsigned int GetLParam();

	protected:
		HWND			m_pHwnd;
		unsigned int    m_iWParam;
		unsigned int    m_iLParam;
	};

	typedef std::shared_ptr<EWindow> EWindowPtr;
};


#endif //__EWindow_h__