//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// SpriteFontLoaderDX11
//
// This class provides a simple static interface for cached copies of a 
// SpriteFontDX11 that match the desired parameters.
//--------------------------------------------------------------------------------
#ifndef __ArkFontLoader_h__
#define __ArkFontLoader_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSpriteFont11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFontLoader
	{
	public:
		ArkFontLoader();
		~ArkFontLoader();

		static SpriteFontPtr LoadFont( std::wstring& fontName, float fontSize, UINT fontStyle, bool antiAliased );

	protected:

		static std::vector< SpriteFontPtr > FontList;
	};

}

//--------------------------------------------------------------------------------
#endif // SpriteFontLoaderDX11_h
//--------------------------------------------------------------------------------