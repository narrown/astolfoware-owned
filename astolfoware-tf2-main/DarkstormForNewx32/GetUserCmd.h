#pragma once
#include "SDK.h"


typedef CUserCmd* (__fastcall* UserCmd_t)(void*, void*, int);
inline UserCmd_t oUserCmd;
CUserCmd* __fastcall Hooked_GetUserCmd(void* ecx, void* edx, int sequence_number);