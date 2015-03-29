//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------
#ifndef __EMouseMsg_h__
#define __EMouseMsg_h__
//--------------------------------------------------------------------------------
#include "EWindow.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EMouseMsg : public EWindow
	{
	public:
		EMouseMsg( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EMouseMsg( );

		bool ControlKeyDown();
		bool LButtonDown();
		bool MButtonDown();
		bool RButtonDown();
		bool ShiftKeyDown();

		int GetX();
		int GetY();
	};

	typedef std::shared_ptr<EMouseMsg> EMouseMsgPtr;
};
//--------------------------------------------------------------------------------
#endif // EvtMouseMsg_h
