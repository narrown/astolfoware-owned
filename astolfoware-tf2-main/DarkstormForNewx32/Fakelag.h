#pragma once
#include "SDK.h"

class CFakeLag
{
public:
	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
};
inline CFakeLag gFakelag;