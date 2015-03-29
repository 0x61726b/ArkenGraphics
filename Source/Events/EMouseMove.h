//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseRButtonDown.h
//--------------------------------------------------------------------------------
#ifndef __EMouseMove_h__
#define __EMouseMove_h__
//--------------------------------------------------------------------------------
#include "EMouseMsg.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EMouseMove : public EMouseMsg
	{
	public:
		EMouseMove( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EMouseMove( );

		virtual std::wstring GetEventName( );
		virtual eEvent GetEventType( );
	};

	typedef std::shared_ptr<EMouseMove> EMouseMovePtr;
};
//--------------------------------------------------------------------------------
#endif // EvtMouseRButtonDown_h
