//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EWindowResize.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "EWindowResize.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EWindowResize::EWindowResize( HWND hwnd,unsigned int wparam,long lparam )
	: EWindow(hwnd,wparam,lparam)
{
}
//--------------------------------------------------------------------------------
EWindowResize::~EWindowResize()
{
}
//--------------------------------------------------------------------------------
std::wstring EWindowResize::GetEventName()
{
	return std::wstring(L"EWindow_Resize");
}
//--------------------------------------------------------------------------------
eEvent EWindowResize::GetEventType()
{
	return WINDOW_RESIZE;
}
//--------------------------------------------------------------------------------
int EWindowResize::NewWidth()
{
	return( m_iLParam & 0xFFFF );
}
//--------------------------------------------------------------------------------
int EWindowResize::NewHeight()
{
	return( ( m_iLParam & 0xFFFF0000 ) >> 16 );
}
//--------------------------------------------------------------------------------
bool EWindowResize::IsMaxHide()	// Message is sent to all pop-up windows when some other window is maximized.
{
	if ( m_iWParam == SIZE_MAXHIDE )
		return( true );
	else
		return( false );
}
//--------------------------------------------------------------------------------
bool EWindowResize::IsMaximized()	// The window has been maximized.
{
	if ( m_iWParam == SIZE_MAXIMIZED )
		return( true );
	else
		return( false );
}
//--------------------------------------------------------------------------------
bool EWindowResize::IsMaxShow()	// Message is sent to all pop-up windows when some other window has been restored to its former size.
{
	if ( m_iWParam == SIZE_MAXSHOW )
		return( true );
	else
		return( false );
}
//--------------------------------------------------------------------------------
bool EWindowResize::IsMinimized() // The window has been minimized.
{
	if ( m_iWParam == SIZE_MINIMIZED )
		return( true );
	else
		return( false );
}
//--------------------------------------------------------------------------------
bool EWindowResize::IsRestored()	// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
{
	if ( m_iWParam == SIZE_RESTORED )
		return( true );
	else
		return( false );
}
//--------------------------------------------------------------------------------