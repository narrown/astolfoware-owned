#pragma once
#include "SDK.h"

typedef void(__fastcall* oSendMove_t)(void*, void*);
inline oSendMove_t oSendMove = nullptr;
void __fastcall Hooked_CLSendMove(void* ecx, void* edx);