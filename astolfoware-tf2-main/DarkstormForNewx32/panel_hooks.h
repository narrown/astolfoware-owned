#pragma once
#include "SDK.h"

namespace panel_hooks {
	namespace paint {
		typedef void(__fastcall* PaintTraverse_t)(void*, void*, unsigned int, bool, bool);
		inline PaintTraverse_t original;
		void __fastcall detour(void* pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	}

	void init();
}