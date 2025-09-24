#include "InCond.h"
#include "Enums.h"
#include "SigScanner.h"
#include <stdio.h>
#include <intrin.h>
#include "MinHook/MinHook.h"
bool __fastcall Hooked_InCond(void* ecx, void* edx, int cond)
{
	if (cond == TF_COND_ZOOMED)
	{
		static const auto dwPlayerShouldDraw = gSignature.GetClientSignature("E8 ? ? ? ? 84 C0 74 05 5E") + 0x5;
		static const auto dwWearableShouldDraw = gSignature.GetClientSignature("E8 ? ? ? ? 84 C0 0F 85 A9 01 00 00 6A 03") + 0x5;

		const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

		if (dwRetAddr == dwPlayerShouldDraw || dwRetAddr == dwWearableShouldDraw)
			return false;
	}
	return oInCond(ecx, edx, cond);
}

void InCond_Initialize()
{
	static DWORD Signature = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC 08 56 57 8B 7D 08 8B F1 83 FF 20");
	auto Nogger = reinterpret_cast<InCond_FN>(Signature);
	MH_CreateHook(Nogger, &Hooked_InCond, reinterpret_cast<void**>(&oInCond));
}