#include "SDK.h"
#include <Windows.h>
#include <thread>
#include "SigScanner.h"
#include "CNetVars.h"
#include "DrawManager.h"
#include "MinHook/MinHook.h"
#include "CEntry.h"
#include "Rage.h"
#include "Menu.h"

void DoMenuStuff(LPVOID lpArguments)
{
	InputSys::Get().Initialize();

	InputSys::Get().RegisterHotkey(VK_INSERT, [lpArguments]()
		{
			Menu::Get().Toggle();
			try
			{
				//gConfig.SaveSettings(".\\Dumpster-Fire.json");
			}
			catch (...)
			{

			}
		});

	InputSys::Get().RegisterHotkey(VK_LBUTTON, [lpArguments]()
		{
			Menu::Get().Click();
		});

	Font::Get().Create();
	Menu::Get().Init();
}

DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	gMarkingSystem.LoadFromFile();
	entry::init();
	DoMenuStuff(lpArguments);
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	entry::uninit(lpArguments);
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, dwMainThread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}