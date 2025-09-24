#pragma once
#include "SDK.h"

typedef bool(__fastcall* DoPostScreenSpaceEffects_fn)(void*,void*, const CViewSetup*);
inline DoPostScreenSpaceEffects_fn oDoPostScreenSpaceEffects;
bool __fastcall Hooked_DoPostScreenSpaceEffects(void* ecx, void* edx, const CViewSetup* view);