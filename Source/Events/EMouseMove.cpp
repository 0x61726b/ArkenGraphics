//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "EMouseMove.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EMouseMove::EMouseMove( HWND hwnd, unsigned int wparam, long lparam )
: EMouseMsg( hwnd, wparam, lparam )
{
}
//--------------------------------------------------------------------------------
EMouseMove::~EMouseMove( )
{
}
//--------------------------------------------------------------------------------
std::wstring EMouseMove::GetEventName( )
{
	return( std::wstring( L"mouse_move" ) );
}
//--------------------------------------------------------------------------------
eEvent EMouseMove::GetEventType( )
{
	return( SYSTEM_MOUSE_MOVE );
}
//--------------------------------------------------------------------------------
