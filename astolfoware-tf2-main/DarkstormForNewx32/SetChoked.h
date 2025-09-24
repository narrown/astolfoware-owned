#pragma once
#include "SDK.h"

typedef void(__fastcall* SetChoked_FN)(CNetChannel*, void*);
inline SetChoked_FN oSetChoked;
void __fastcall Hooked_SetChoked(CNetChannel* ecx, void* edx);