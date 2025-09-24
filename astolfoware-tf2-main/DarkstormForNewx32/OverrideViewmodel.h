#pragma once
#include "SDK.h"

typedef void(__fastcall* Nigger_FN)(void*, void*, CViewSetup*);
inline Nigger_FN oOverrideViewmodel;
void __fastcall Hooked_OverrideViewModel(void* ecx, void* edx, CViewSetup* view);

typedef bool(__fastcall* Nigger_FN2)(void*, void*, CViewSetup*);
inline Nigger_FN2 oOverrideViewmodelBool;
bool __fastcall Hooked_OverrideViewModelBool(void* ecx, void* edx, CViewSetup* view);