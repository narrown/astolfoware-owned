#pragma once


/*
inline SEOHook::Func Func;
	using fn = bool(__fastcall *)(void *, void *, ETFCond cond);
	bool __fastcall Hook(void *ecx, void *edx, ETFCond cond);
*/

typedef bool(__fastcall* InCond_FN)(void*, void*, int);
inline InCond_FN oInCond;
bool __fastcall Hooked_InCond(void* ecx, void* edx, int cond);
void InCond_Initialize();