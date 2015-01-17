//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EKeyboard.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "EKeyboard.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
EKeyboard::EKeyboard( HWND hwnd, unsigned int wParam,long lParam )
	: EWindow( hwnd,wParam,lParam )
{

}
//--------------------------------------------------------------------------------
EKeyboard::~EKeyboard()
{
}
//--------------------------------------------------------------------------------
unsigned int EKeyboard::GetCharacterCode()
{
	return(m_iWParam);
}
//--------------------------------------------------------------------------------
unsigned int EKeyboard::GetRepeatCount()
{
	return(0xffff&m_iLParam);
}
//--------------------------------------------------------------------------------
unsigned int EKeyboard::GetScanCode()
{
	return((0xf0000&m_iLParam)>>16);
}
//--------------------------------------------------------------------------------
bool EKeyboard::GetExtendedKey()
{
	return((0x01000000&m_iLParam) > 0);
}
//--------------------------------------------------------------------------------
bool EKeyboard::GetContextCode()
{
	return((0x20000000&m_iLParam) > 0);
}
//--------------------------------------------------------------------------------
bool EKeyboard::GetPreviousKeyState()
{
	return((0x40000000&m_iLParam) > 0);
}
//--------------------------------------------------------------------------------
bool EKeyboard::GetTransitionState()
{
	return((0x80000000&m_iLParam) > 0);
}
//--------------------------------------------------------------------------------