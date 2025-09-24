#include "DrawModelExecute.h"
#include "Vars.h"

void __stdcall Hooked_DrawModelExecute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld) {
	return oDME(state, pInfo, pCustomBoneToWorld);
}