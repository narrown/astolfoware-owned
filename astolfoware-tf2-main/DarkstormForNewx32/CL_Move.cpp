#include "CL_Move.h"
#include "Vars.h"
#include "MinHook/MinHook.h"
#include "Utils.h"
#include "CreateMove.h"
#include "Networking.h"
struct GlobalsBackup_t
{
	float CLFrameTime = 0.f;
	float FrameTime = 0.f;
	float CurrentTime = 0.f;
	int TickCount = 0;

	inline void Store()
	{
		CLFrameTime = gInts.ClientState->m_frameTime;
		FrameTime = gInts.Globals->frametime;
		CurrentTime = gInts.Globals->curtime;
		TickCount = gInts.Globals->tickcount;
	}

	inline void Restore()
	{
		gInts.ClientState->m_frameTime = CLFrameTime;
		gInts.Globals->frametime = FrameTime;
		gInts.Globals->curtime = CurrentTime;
		gInts.Globals->tickcount = TickCount;
	}
};

GlobalsBackup_t State;

void RemoveInputDelay(bool FinalTick)
{
	if (gInts.ClientState->m_NetChannel && gInts.ClientState->m_NetChannel->IsLoopback())
		return;

	if (!gInts.Engine->IsInGame())
		return;

	GlobalsBackup_t Backup;

	Backup.Store();
	{
		oReadPackets(FinalTick);
		State.Store();
	}
	Backup.Restore();
}

bool ShouldReadPackets()
{
	if (gInts.ClientState->m_NetChannel && gInts.ClientState->m_NetChannel->IsLoopback())
		return true;

	if (!gInts.Engine->IsInGame())
		return true;

	State.Restore();
	return false;
}

void __cdecl Hooked_CLReadPackets(bool final_tick)
{
	if (ShouldReadPackets())
		oReadPackets(final_tick);
}

void Initialize_CLReadPackets()
{
	static DWORD Signature = Memory::find_signature<DWORD>("engine.dll", "55 8B EC 83 EC 34 8B 0D ? ? ? ?");
	auto CastedReadPacketsSig = reinterpret_cast<oCLReadPackets_FN>(Signature);
	MH_CreateHook(CastedReadPacketsSig, &Hooked_CLReadPackets, reinterpret_cast<LPVOID*>(&oReadPackets));
}

void __cdecl Hooked_CLMove(float accumulated_extra_samples, bool final_tick)
{

	//oCLMove(accumulated_extra_samples, final_tick);
	gNetworking.CL_Move(accumulated_extra_samples, final_tick);

	if (Vars::DT::DoubleTap)
	{

		if (Vars::DT::shifting)
		{
			while (Vars::DT::Shifted > Vars::DT::ticks)
			{
				//accumulated_extra_samples = 0.0f;
				gNetworking.CL_Move(accumulated_extra_samples, Vars::DT::Shifted == 2 + 1);
				//i::cvar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "[Doubletap] CL_Move( %i - %.8f, %d )\n", global_info.shifted, accumulated_extra_samples, global_info.shifted == global_info.ticks + 1 );
				Vars::DT::Shifted--;
				gInts.Engine->FireEvents(); 
			}

			Vars::DT::shifting = false;
		}
	}
	
}
#include <stdio.h>
#include <intrin.h>
/* What does this even do? */
float __fastcall Hooked_CLFireEvents(void* ecx, void* edx)
{
	//static auto originalFn = Func.Original<fn>();
	static DWORD dwGetTime = gSignature.GetEngineSignature("D9 43 ? DF F1");

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == (dwGetTime))
		return FLT_MAX;

	return oFireEvents(ecx, edx);
}

void Initalize_HookedCLFireEvents()
{
	static DWORD dwCLFireEvents = gSignature.GetEngineSignature("E8 ? ? ? ? 83 EC 10") + 0x1;
	DWORD dwEstimate = *(PDWORD)dwCLFireEvents + dwCLFireEvents + 4;
	CLFireEvents_FN CLFireEvents = reinterpret_cast<CLFireEvents_FN>(dwEstimate);

	MH_CreateHook(CLFireEvents, &Hooked_CLFireEvents, reinterpret_cast<void**>(&oFireEvents));
}

