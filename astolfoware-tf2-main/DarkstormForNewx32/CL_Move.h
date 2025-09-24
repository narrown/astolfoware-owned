#pragma once
#include "SDK.h"

typedef void(__cdecl* MoveFN)(float, bool);
inline MoveFN oCLMove = nullptr;
void __cdecl Hooked_CLMove(float accumulated_extra_samples, bool final_tick);

//using fn = float(__fastcall*)(void*, void*);
typedef float(__fastcall* CLFireEvents_FN)(void*, void*);
inline CLFireEvents_FN oFireEvents;
float __fastcall Hooked_CLFireEvents(void* ecx, void* edx);
void Initalize_HookedCLFireEvents();

/*
void __cdecl hooks::cl_read_packets::function(bool final_tick) {
	if (!config->m_misc.m_reduce_input_delay) {
		return original_function(final_tick);
	}

	 
	if (misc->should_read_packets())
		original_function(final_tick);
}

void hooks::cl_read_packets::init() {
	auto target = util::find_signature< fn >("engine.dll", "55 8B EC 83 EC 34 8B 0D ? ? ? ?");
	MH_CreateHook(target, &function, reinterpret_cast<LPVOID*>(&original_function));
}

*/

typedef void(__cdecl* oCLReadPackets_FN)(bool);
inline oCLReadPackets_FN oReadPackets = nullptr;
void __cdecl Hooked_CLReadPackets(bool final_tick);
void Initialize_CLReadPackets();

typedef void(__fastcall* ProcessPackets_fn)(void*, void*, void*, bool);
inline ProcessPackets_fn oProcessPackets = nullptr;
void __fastcall Hooked_ProcessPackets(void* ecx, void* edx, void* packet, bool header);
void init_processpackets();