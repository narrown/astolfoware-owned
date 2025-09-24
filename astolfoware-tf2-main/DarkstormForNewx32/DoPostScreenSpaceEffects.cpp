#include "DoPostScreenSpaceEffects.h"
#include "Visuals.h"
/*
auto target = util::get_virtual< fn >(ctx->m_client_mode, 39u);
	MH_CreateHook(target, &function, reinterpret_cast<LPVOID*>(&original_function));
*/
bool __fastcall Hooked_DoPostScreenSpaceEffects(void* ecx, void* edx, const CViewSetup* view) {
	//gVisuals.ProjectileTrajectory();

	return oDoPostScreenSpaceEffects(ecx, edx, view);
}