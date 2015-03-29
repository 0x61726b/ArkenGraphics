//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseRButtonDown.h
//--------------------------------------------------------------------------------
#ifndef __EMouseRButtonUp_h__
#define __EMouseRButtonUp_h__
//--------------------------------------------------------------------------------
#include "EMouseMsg.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EMouseRButtonUp : public EMouseMsg
	{
	public:
		EMouseRButtonUp( HWND hwnd, unsigned int wparam, long lparam );
		virtual ~EMouseRButtonUp( );

		virtual std::wstring GetEventName( );
		virtual eEvent GetEventType( );
	};

	typedef std::shared_ptr<EMouseRButtonUp> EMouseRButtonUpPtr;
};
//--------------------------------------------------------------------------------
#endif // EvtMouseRButtonDown_h
