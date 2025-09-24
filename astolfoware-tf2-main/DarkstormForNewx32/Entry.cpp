#include "CEntry.h"
#include "Interface.h"
#include "Hooks.h"
#include "CConsole.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Rage.h"

void entry::init()
{
    sigs::init();
    interfaces::init();
    hooks::init();
    i::engine->ClientCmd_Unrestricted("toggleconsole");
    i::cvars->ConsolePrintf("[astolfoware.cc] injected!");
}

void entry::uninit(LPVOID lpArguments)
{
	//Hooks->Uninitalize(lpArguments);
    hooks::uninit(lpArguments);
}
