#pragma once
#include "SDK.h"


typedef void(__fastcall* oSetAbsVelocity_fn)(void*, void*, const Vector&);
inline oSetAbsVelocity_fn oSetAbsVelocity;
void __fastcall Hooked_SetAbsVelocity(void* ecx, void* edx, const Vector &vecAbsVelocity);

/*
MAKE_HOOK(
	C_BaseEntity_EstimateAbsVelocity, Signatures::C_BaseEntity_EstimateAbsVelocity.Get(),
	void, __fastcall, void *ecx, void *edx, Vector &vel)
*/
typedef void(__fastcall* oEstiamteAbsVelocity_fn)(void*, void*, Vector&);
inline oEstiamteAbsVelocity_fn oEstimateAbsVelocity;
void __fastcall Hooked_EstimateAbsVelocity(void* ecx, void* edx, Vector& vel);