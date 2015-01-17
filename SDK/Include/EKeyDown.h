//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EKeyDown.h
//--------------------------------------------------------------------------------
#ifndef __EKeyDown_h__
#define __EKeyDown_h__
//--------------------------------------------------------------------------------
#include "EKeyboard.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EKeyDown : public EKeyboard
	{
	public:
		EKeyDown( HWND hwnd, unsigned int wParam, long lParam );
		virtual ~EKeyDown();

		virtual std::wstring GetEventName();
		virtual eEvent GetEventType();
	};
	typedef std::shared_ptr<EKeyDown> EKeyDownPtr;
};
#endif //__EKeyDown
