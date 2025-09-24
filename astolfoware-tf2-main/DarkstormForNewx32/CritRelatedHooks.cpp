#include "CritRelatedHooks.h"
#include "CreateMove.h"
#include "Vars.h"
#include "CritHack.h"
void __fastcall Hooked_CalcIsAttackCritical(CBaseCombatWeapon* this_, void* edx) {
	//if (!gLocalPlayer || !gLocalWeapon || gLocalWeapon != this_)
	//	return oCalcIsAttackCritical(this_, edx);

	oCalcIsAttackCritical(this_, edx);
}

void __fastcall Hooked_AddToCritBucket(CBaseCombatWeapon* this_, void* edx, float damage)
{
	//if (this_ == gLocalWeapon && !gCritHack.m_is_going_through_crit_cmds)
	//	if (Vars::CritHax::finished_last_shot) //easy fix for non-melee.
	return oAddToCritBucket(this_, edx, damage);
}