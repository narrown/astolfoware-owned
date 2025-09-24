#pragma once
#include "SDK.h"
#include "DrawManager.h"

typedef void(__fastcall* oScreenSizeChanged_fn)(void*, void*, int, int);
inline oScreenSizeChanged_fn oScreenSizeChanged;
void __fastcall Hooked_OnScreenSizeChanged(void* ecx, void* edx, int oldW, int oldH);