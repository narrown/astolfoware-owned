#pragma once
#include "SDK.h"
#include "DrawManager.h"


namespace surface_hooks {
	namespace on_screen_size_changed {
		typedef void(__fastcall* oScreenSizeChanged_fn)(void*, void*, int, int);
		inline oScreenSizeChanged_fn original;
		void __fastcall detour(void* ecx, void* edx, int oldW, int oldH);
	}

	void init();
}