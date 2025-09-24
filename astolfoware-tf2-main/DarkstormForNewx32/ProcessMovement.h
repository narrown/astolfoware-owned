#pragma once
#include "SDK.h"

typedef void(__fastcall* ProcessMovement_fn)(void*, void*, C_BasePlayer*, CMoveData*);
inline ProcessMovement_fn oProcessMovement;
void __fastcall Hooked_ProcessMovement(void* ecx, void* edx, C_BasePlayer* pPlayer, CMoveData* pMove);
