#include "Fonts.h"
#include "SDK.h"

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

void Font::Create()
{
	Overlay = i::surface->CreateFont();
	MenuText = i::surface->CreateFont();
	ESP = i::surface->CreateFont();

	i::surface->SetFontGlyphSet(Overlay, "Tahoma", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
	i::surface->SetFontGlyphSet(MenuText, "Tahoma", 12, 550, 0, 0, FONTFLAG_ANTIALIAS);
	i::surface->SetFontGlyphSet(ESP, "Arial", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);
}

void Font::Update()
{
	/* 0 it out */
	Overlay = 0x0;
	MenuText = 0x0;
	ESP = 0x0;

	Overlay = i::surface->CreateFont();
	MenuText = i::surface->CreateFont();
	ESP = i::surface->CreateFont();

	i::surface->SetFontGlyphSet(Overlay, "Tahoma", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
	i::surface->SetFontGlyphSet(MenuText, "Tahoma", 12, 550, 0, 0, FONTFLAG_ANTIALIAS);
	i::surface->SetFontGlyphSet(ESP, "Arial", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);
}