//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "EMouseLeave.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EMouseLeave::EMouseLeave( HWND hwnd, unsigned int wparam, long lparam )
: EMouseMsg( hwnd, wparam, lparam )
{
}
//--------------------------------------------------------------------------------
EMouseLeave::~EMouseLeave( )
{
}
//--------------------------------------------------------------------------------
std::wstring EMouseLeave::GetEventName( )
{
	return( std::wstring( L"mouse_leave" ) );
}
//--------------------------------------------------------------------------------
eEvent EMouseLeave::GetEventType( )
{
	return( SYSTEM_MOUSE_LEAVE );
}
//--------------------------------------------------------------------------------
