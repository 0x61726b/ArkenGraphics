//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EKeyDown.h
//--------------------------------------------------------------------------------
#ifndef __EKeyUp_h__
#define __EKeyUp_h__
//--------------------------------------------------------------------------------
#include "EKeyboard.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EKeyUp : public EKeyboard
	{
	public:
		EKeyUp( HWND hwnd, unsigned int wParam, long lParam );
		virtual ~EKeyUp();

		virtual std::wstring GetEventName();
		virtual eEvent GetEventType();
	};
	typedef std::shared_ptr<EKeyUp> EKeyUpPtr;
};
#endif //__EKeyDown
