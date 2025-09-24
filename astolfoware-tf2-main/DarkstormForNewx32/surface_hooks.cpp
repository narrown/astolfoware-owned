#include "surface_hooks.h"
#include "Fonts.h"
#include "MinHook/MinHook.h"

using namespace surface_hooks;

void __fastcall on_screen_size_changed::detour(void* ecx, void* edx, int oldW, int oldH)
{
	original(ecx, edx, oldW, oldH);

	gDraw.ReloadFonts();
	gDraw.UpdateScreenSize();
	Font::Get().Update();
}
void surface_hooks::init() {
	MH_CreateHook(Memory::GetVirtual(i::surface, 111), &on_screen_size_changed::detour, reinterpret_cast<void**>(&on_screen_size_changed::original));

}