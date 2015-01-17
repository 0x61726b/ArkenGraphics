//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IWndProc.h
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef __IWndProc_h__
#define __IWndProc_h__

namespace Arkeng
{
	class IWndProc
	{
	public:
		virtual LRESULT WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) = 0;
		virtual void BeforeRegisterWindowClass(WNDCLASSEX &wc) = 0;
	};
}

#endif //__IWndProc_h__