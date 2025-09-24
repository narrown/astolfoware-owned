#include "engine_hooks.h"
#include "MinHook/MinHook.h"
#include "Networking.h"
#include "Vars.h"
#include <stdio.h>
#include <intrin.h>
#include "Chams.h"

using namespace engine_hooks;

void __cdecl CL_Move::detour(float accumulated_extra_samples, bool final_tick)
{
	gNetworking.CL_Move(accumulated_extra_samples, final_tick);

	if (Vars::DT::DoubleTap)
	{

		if (Vars::DT::shifting)
		{
			while (Vars::DT::Shifted > Vars::DT::ticks)
			{
				//accumulated_extra_samples = 0.0f;
				gNetworking.CL_Move(0, Vars::DT::Shifted == 1); // apparently this is the "engine" way of doing DT.. credz pixy
				//i::cvar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "[Doubletap] CL_Move( %i - %.8f, %d )\n", global_info.shifted, accumulated_extra_samples, global_info.shifted == global_info.ticks + 1 );
				Vars::DT::Shifted--;
				i::engine->FireEvents(); // find a better way of doing this
			}

			Vars::DT::shifting = false;

			return; //?
		}
	}
}


/* What does this even do? */
float __fastcall CL_FireEvents::detour(void* ecx, void* edx)
{
	//static auto originalFn = Func.Original<fn>();
	static DWORD dwGetTime = gSignature.GetEngineSignature("D9 43 ? DF F1");

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == (dwGetTime))
		return FLT_MAX;

	return original(ecx, edx);
}

void __fastcall SceneEnd::detour(void* ecx, void* edx) {
	original(ecx, edx);

	gChams.Initialize();
}

void __cdecl check_for_pure_server_whitelist::detour(void** files) {
	if (Vars::Misc::PureBypass)
		return;

	original(files);
}

void engine_hooks::init() {
	/* this is fucking disgusting */
	/* todo: move the sig to sigmanager */
	static DWORD dwCLFireEvents = gSignature.GetEngineSignature("E8 ? ? ? ? 83 EC 10") + 0x1;
	DWORD dwEstimate = *(PDWORD)dwCLFireEvents + dwCLFireEvents + 4;
	CL_FireEvents::CLFireEvents_FN CLFireEvents = reinterpret_cast<CL_FireEvents::CLFireEvents_FN>(dwEstimate);

	auto CLMoveTarget = reinterpret_cast<CL_Move::MoveFN>(sigs::cl_move);
	auto CLPureTarget = reinterpret_cast<check_for_pure_server_whitelist::fn>(sigs::sv_pure);

	MH_CreateHook(CLFireEvents, &CL_FireEvents::detour, reinterpret_cast<void**>(&CL_FireEvents::original));
	MH_CreateHook(CLMoveTarget, &CL_Move::detour, reinterpret_cast<LPVOID*>(&CL_Move::original));
	MH_CreateHook(CLPureTarget, &check_for_pure_server_whitelist::detour, reinterpret_cast<LPVOID*>(&check_for_pure_server_whitelist::original));
	//MH_CreateHook(Memory::GetVirtual(i::renderview, 9), &SceneEnd::detour, reinterpret_cast<LPVOID*>(&SceneEnd::original));
}