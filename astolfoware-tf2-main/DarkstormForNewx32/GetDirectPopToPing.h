#pragma once
#include "SDK.h"

typedef unsigned int uint32;
typedef uint32 SteamNetworkingPOPID;

typedef int(__fastcall* oDirectPingToPOP_t)(void*, void*, SteamNetworkingPOPID);
inline oDirectPingToPOP_t oDirectPing;
int __fastcall Hooked_DirectPingToPOP(void* ecx, void* edx, SteamNetworkingPOPID PopID);
