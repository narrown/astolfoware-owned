#pragma once
#include "SDK.h"


class CCrithack
{
public:
	int GetCritCommand(CUserCmd* pCmd, CBaseCombatWeapon* pWeapon, bool bCrit);
	void init(CUserCmd* pCmd);
};

inline CCrithack Crithack;