#include "Hooks.h"
#include "MinHook/MinHook.h"
#include "CConsole.h"
#include "client.h"
#include "clientmode.h"
#include "engine_hooks.h"
#include "surface_hooks.h"
#include "input_hooks.h"
#include "panel_hooks.h"
#include "enginevgui_hooks.h"
void hooks::uninit(LPVOID lpArguments)
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
}

void hooks::init()
{

	MH_Initialize();
	{
		chlclient_hooks::init();
		clientmode_hooks::init();
		engine_hooks::init();
		surface_hooks::init();
		input_hooks::init();
		panel_hooks::init();
		enginevgui_hooks::init();
	}
	MH_EnableHook(MH_ALL_HOOKS);
}