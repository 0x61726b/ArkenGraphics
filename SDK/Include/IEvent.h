//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IEvent.h
//--------------------------------------------------------------------------------
#ifndef __IEvent_h__
#define __IEvent_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
//--------------------------------------------------------------------------------

namespace Arkeng
{
	enum eEvent
	{
		SYSTEM_KEYBOARD_KEYDOWN,
		SYSTEM_KEYBOARD_KEYUP,
		SYSTEM_KEYBOARD_CHAR,
		WINDOW_RESIZE,
		NUM_EVENTS
	};
	class IEvent
	{
	public:
		virtual std::wstring GetEventName() = 0;
		virtual eEvent GetEventType() = 0;
	};

	typedef std::shared_ptr<IEvent> EventPtr;
};


#endif // __IEvent_h__