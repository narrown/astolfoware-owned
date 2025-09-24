#pragma once
#include "SDK.h"
#include "MinHook/MinHook.h"

namespace input_hooks {
	namespace get_user_cmd {
		typedef CUserCmd* (__fastcall* UserCmd_t)(void*, void*, int);
		inline UserCmd_t original;
		CUserCmd* __fastcall detour(void* ecx, void* edx, int sequence_number);
	}

	void init();
}