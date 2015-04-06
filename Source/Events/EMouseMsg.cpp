//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EMouseMsg.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "EMouseMsg.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EMouseMsg::EMouseMsg( HWND hwnd, unsigned int wparam, long lparam )
	: EWindow( hwnd, wparam, lparam )
{
}
//--------------------------------------------------------------------------------
EMouseMsg::~EMouseMsg( )
{
}
//--------------------------------------------------------------------------------
bool EMouseMsg::ControlKeyDown()
{
	return( ( MK_CONTROL & m_iWParam ) > 0 );
}
//--------------------------------------------------------------------------------
bool EMouseMsg::LButtonDown()
{
	return( ( MK_LBUTTON & m_iWParam ) > 0 );
}
//--------------------------------------------------------------------------------
bool EMouseMsg::MButtonDown()
{
	return( ( MK_MBUTTON & m_iWParam ) > 0 );
}
//--------------------------------------------------------------------------------
bool EMouseMsg::RButtonDown()
{
	return( ( MK_RBUTTON & m_iWParam ) > 0 );
}
//--------------------------------------------------------------------------------
bool EMouseMsg::ShiftKeyDown()
{
	return( ( MK_SHIFT & m_iWParam ) > 0 );
}
//--------------------------------------------------------------------------------
int EMouseMsg::GetX()
{
	return( 0xffff & m_iLParam );
}
//--------------------------------------------------------------------------------
int EMouseMsg::GetY()
{
	return( ( 0xffff0000 & m_iLParam ) >> 16 );
}
//--------------------------------------------------------------------------------