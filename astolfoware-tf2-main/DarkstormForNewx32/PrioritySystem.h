#pragma once
#include "SDK.h"

class CPrioritySystem
{
public:
	bool ShouldBeIgnored(CBaseEntity* pPlayer);
	std::string ShouldBeRage(int playerid); /* for later */
};

inline CPrioritySystem gPriority;