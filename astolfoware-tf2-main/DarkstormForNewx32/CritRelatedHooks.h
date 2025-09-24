#pragma once
#include "SDK.h"


/* calc is attack critical */
typedef void(__fastcall* Calc_fn)(CBaseCombatWeapon*, void*);
inline Calc_fn oCalcIsAttackCritical;
void __fastcall Hooked_CalcIsAttackCritical(CBaseCombatWeapon* this_, void* edx);

/* AddToCritBucket */
typedef void(__fastcall* AddToCrit_t)(CBaseCombatWeapon*, void*, float);
inline AddToCrit_t oAddToCritBucket;
void __fastcall Hooked_AddToCritBucket(CBaseCombatWeapon* this_, void* edx, float damage);
