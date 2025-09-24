#include "OnScreenSizeChanged.h"
#include "Fonts.h"
void __fastcall Hooked_OnScreenSizeChanged(void* ecx, void* edx, int oldW, int oldH)
{
	oScreenSizeChanged(ecx, edx, oldW, oldH);

	gDraw.ReloadFonts();
	gDraw.UpdateScreenSize();
	Font::Get().Update();
}