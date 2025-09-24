#pragma once
#include "SDK.h"

typedef void(__fastcall* fn2)(void* ecx, void* edx, int);
inline fn2 oPaint;
void __fastcall Hooked_EngineVGUIPaint(void* ecx, void* edx, int mode);