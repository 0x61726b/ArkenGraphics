//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "EMouseRButtonUp.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EMouseRButtonUp::EMouseRButtonUp( HWND hwnd, unsigned int wparam, long lparam )
: EMouseMsg( hwnd, wparam, lparam )
{
}
//--------------------------------------------------------------------------------
EMouseRButtonUp::~EMouseRButtonUp( )
{
}
//--------------------------------------------------------------------------------
std::wstring EMouseRButtonUp::GetEventName( )
{
	return( std::wstring( L"rbutton_up" ) );
}
//--------------------------------------------------------------------------------
eEvent EMouseRButtonUp::GetEventType( )
{
	return( SYSTEM_RBUTTON_UP );
}
//--------------------------------------------------------------------------------
