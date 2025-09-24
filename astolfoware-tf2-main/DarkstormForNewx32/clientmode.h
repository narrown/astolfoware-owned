#pragma once
#include "SDK.h"

namespace clientmode_hooks {
	namespace create_move {
		typedef bool(__fastcall* CreateMove_t)(void*, void*, float, CUserCmd*);
		inline CreateMove_t original;
		bool __fastcall detour(void* ClientMode, void* edx, float flInputSampleFrametime, CUserCmd* pCmd);
	}
	// OverrideViewModel
	namespace override_view_model {
		typedef void(__fastcall* Nigger_FN)(void*, void*, CViewSetup*);
		inline Nigger_FN original;
		void __fastcall detour(void* ecx, void* edx, CViewSetup* view);
	}
	namespace override_view_model_bool {
		typedef bool(__fastcall* Nigger_FN2)(void*, void*, CViewSetup*);
		inline Nigger_FN2 original;
		bool __fastcall detour(void* ecx, void* edx, CViewSetup* view);
	}
	//DoPostScreenSpaceEffects
	namespace do_post_screen_space_effects {
		typedef bool(__fastcall* DoPostScreenSpaceEffects_fn)(void*, void*, const CViewSetup*);
		inline DoPostScreenSpaceEffects_fn original;
		bool __fastcall detour(void* ecx, void* edx, const CViewSetup* view);
	}
	void init();
}