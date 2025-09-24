#include "GetFont.h"

unsigned long __fastcall Hooked_GetFont(void* ecx, void* edx, const char* fontName, bool proportional)
{
	//oGetFont(ecx, edx, fontName, proportional);
	gInts.Cvars->ConsolePrintf("GetFont Called with the fontName:");
	gInts.Cvars->ConsolePrintf(fontName);
	return oGetFont(ecx, edx, fontName, proportional);
}