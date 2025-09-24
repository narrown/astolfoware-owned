#pragma once
#include "SDK.h"

class CMisc
{
public:
	void Initalize(CBaseEntity* pLocal, CUserCmd* pCmd);
	void AutoStrafe(CBaseEntity* pLocal, CUserCmd* pCmd);
	void BunnyHop(CBaseEntity* pLocal, CUserCmd* pCmd);
	void DoThirdPerson(); // later
	void DoAntiWarp(CUserCmd* pCmd);
	void DoTauntSpin(CUserCmd* pCmd); // later
	void AutoRocketJump(CUserCmd* pCmd); // later
	void auto_scout_jump(CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd);

	//void BypassPure(); no
};
inline CMisc gMisc;