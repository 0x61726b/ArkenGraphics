//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EKeyboard.h
//--------------------------------------------------------------------------------
#include "EWindow.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EKeyboard : public EWindow
	{
	public:
		EKeyboard( HWND hwnd, unsigned int wParam, long lParam );
		virtual ~EKeyboard();

		unsigned int GetCharacterCode();
		unsigned int GetRepeatCount();
		unsigned int GetScanCode();
		bool GetExtendedKey();
		bool GetContextCode();
		bool GetPreviousKeyState();
		bool GetTransitionState();
	};

	typedef std::shared_ptr<EKeyboard> EKeyboardPtr;
};