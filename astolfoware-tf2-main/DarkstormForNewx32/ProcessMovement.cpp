#include "ProcessMovement.h"
// gamemovement vfunc 1 
void __fastcall Hooked_ProcessMovement(void* ecx, void* edx, C_BasePlayer* pPlayer, CMoveData* pMove) {
	if (!pPlayer || !pMove)
		return oProcessMovement(ecx, edx, pPlayer, pMove);

	if (!gInts.MoveHelper)
		return oProcessMovement(ecx, edx, pPlayer, pMove);

	gInts.MoveHelper->SetHost(pPlayer);

	oProcessMovement(ecx, edx, pPlayer, pMove);

	gInts.MoveHelper->SetHost(nullptr);

}