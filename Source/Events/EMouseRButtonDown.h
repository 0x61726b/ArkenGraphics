//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseRButtonDown.h
//--------------------------------------------------------------------------------
#ifndef __EMouseRButtonDown_h__
#define __EMouseRButtonDown_h__
//--------------------------------------------------------------------------------
#include "EMouseMsg.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EMouseRButtonDown : public EMouseMsg
	{
	public:
		EMouseRButtonDown( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EMouseRButtonDown( );

		virtual std::wstring GetEventName( );
		virtual eEvent GetEventType( );
	};

	typedef std::shared_ptr<EMouseRButtonDown> EMouseRButtonDownPtr;
};
//--------------------------------------------------------------------------------
#endif // EvtMouseRButtonDown_h
