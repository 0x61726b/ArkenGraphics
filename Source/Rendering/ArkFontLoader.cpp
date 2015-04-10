//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderAppSimple.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFontLoader.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
std::vector< SpriteFontPtr > ArkFontLoader::FontList;
//--------------------------------------------------------------------------------
ArkFontLoader::ArkFontLoader()
{
}
//--------------------------------------------------------------------------------
ArkFontLoader::~ArkFontLoader()
{
}
//--------------------------------------------------------------------------------
SpriteFontPtr ArkFontLoader::LoadFont( std::wstring& fontName, float fontSize, UINT fontStyle, bool antiAliased )
{
	SpriteFontPtr pFont = nullptr;

	// Search our cache for existing font objects that match the requested
	// parameters.

	for ( auto pCachedFont : FontList )
	{
		if ( pCachedFont->FontName().compare( fontName ) == 0
			&& pCachedFont->Size() == fontSize
			&& pCachedFont->FontStyle() == fontStyle
			&& pCachedFont->AntiAliased() == antiAliased )
		{
			pFont = pCachedFont;
			break;
		}
	}

	// If the font was not found, then we must create a new one and add 
	// it to the list for next time.

	if ( pFont == nullptr )
	{
		pFont = SpriteFontPtr( new ArkSpriteFont11() );
		pFont->Initialize( fontName, fontSize, fontStyle, antiAliased );
		FontList.push_back( pFont );
	}

	// Regardless of if it was cached or newly created, return the font reference.
	
	return( pFont );
}
//--------------------------------------------------------------------------------
