//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------

#include "Pch.h"
#include "EMouseRButtonDown.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EMouseRButtonDown::EMouseRButtonDown( HWND hwnd, unsigned int wparam, long lparam )
: EMouseMsg( hwnd, wparam, lparam )
{
}
//--------------------------------------------------------------------------------
EMouseRButtonDown::~EMouseRButtonDown( )
{
}
//--------------------------------------------------------------------------------
std::wstring EMouseRButtonDown::GetEventName( )
{
	return( std::wstring( L"rbutton_down" ) );
}
//--------------------------------------------------------------------------------
eEvent EMouseRButtonDown::GetEventType( )
{
	return( SYSTEM_RBUTTON_DOWN );
}
//--------------------------------------------------------------------------------
