#pragma once
#include "SDK.h"

namespace enginevgui_hooks {
	namespace paint {
		typedef void(__fastcall* fn2)(void* ecx, void* edx, int);
		inline fn2 original;
		void __fastcall detour(void* ecx, void* edx, int mode);
	}

	void init();
}