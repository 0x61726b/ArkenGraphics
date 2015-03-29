//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseRButtonDown.h
//--------------------------------------------------------------------------------
#ifndef __EMouseLeave_h__
#define __EMouseLeave_h__
//--------------------------------------------------------------------------------
#include "EMouseMsg.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EMouseLeave : public EMouseMsg
	{
	public:
		EMouseLeave( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EMouseLeave( );

		virtual std::wstring GetEventName( );
		virtual eEvent GetEventType( );
	};

	typedef std::shared_ptr<EMouseLeave> EMouseLeavePtr;
};
//--------------------------------------------------------------------------------
#endif // EvtMouseRButtonDown_h
