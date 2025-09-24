#pragma once
#include "SDKClasses.h"
#include "bitbuf.h"
#include "Memory.h"
#include "ClientClass.h"

class CHLClient
{
public:
	void CreateMove(int sequence_number, float input_sample_frametime, bool active)
	{
		typedef void(__thiscall* original_fn)(void*, int, float, bool);
		return Memory::CallVirtual<original_fn>(this, 21)(this, sequence_number, input_sample_frametime, active);
		//mem::callVirtual<21, void, int, float, bool>(this, sequence_number, input_sample_frametime, active);
	}
	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return Memory::CallVirtual<OriginalFn>(this, 8)(this); //Return the pointer to the head CClientClass.
	}

	bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand)
	{
		return Memory::CallVirtual< bool(__thiscall*)(CHLClient*, bf_write*, int, int, bool) >(this, 23)(this, buf, from, to, isnewcommand);
	}
	bool GetPlayerView(CViewSetup& playerView)
	{
		typedef bool(__thiscall* FN)(PVOID, CViewSetup&);
		return Memory::CallVirtual<FN>(this, 59)(this, playerView);
	}
};