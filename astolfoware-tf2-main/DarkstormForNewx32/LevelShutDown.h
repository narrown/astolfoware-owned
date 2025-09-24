#pragma once
#include "SDK.h"


typedef void(__fastcall* oLevelShutdown_t)(void*, void*);
inline oLevelShutdown_t oLevelShutdown;
void __fastcall Hooked_LevelShutdown(void* ecx, void* edx);