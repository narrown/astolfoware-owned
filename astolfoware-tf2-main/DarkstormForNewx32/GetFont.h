#pragma once
#include "SDK.h"
/*
HFont CScheme::GetFont( const char *fontName, bool proportional )
{
	// First look in the list of aliases...
	return FindFontInAliasList( GetMungedFontName( fontName, tag, proportional ) );
}
*/
typedef unsigned long(__fastcall* oGetFont_FN)(void*, void*, const char*, bool);
inline oGetFont_FN oGetFont;
unsigned long __fastcall Hooked_GetFont(void* ecx, void* edx, const char* fontName, bool proportional);