#pragma once
#include "SDK.h"

typedef void(__stdcall* DMEFn)(const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*);
inline DMEFn oDME;
void __stdcall Hooked_DrawModelExecute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld);
