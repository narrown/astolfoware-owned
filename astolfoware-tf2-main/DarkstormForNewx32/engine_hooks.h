#pragma once
#include "SDK.h"

namespace engine_hooks {
	namespace CL_Move {
		typedef void(__cdecl* MoveFN)(float, bool);
		inline MoveFN original = nullptr;
		void __cdecl detour(float accumulated_extra_samples, bool final_tick);
	}

	namespace CL_FireEvents {
		typedef float(__fastcall* CLFireEvents_FN)(void*, void*);
		inline CLFireEvents_FN original;
		float __fastcall detour(void* ecx, void* edx);
	}

	namespace SceneEnd {
		// index 9 
		typedef void(__fastcall* fn)(void*, void*);
		inline fn original;
		void __fastcall detour(void*, void*);
	}

	namespace check_for_pure_server_whitelist {
		typedef void(__cdecl* fn)(void**);
		inline fn original;
		void __cdecl detour(void** files);
	}

	void init();
}