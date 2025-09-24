#pragma once
#include "SDK.h"
#include <math.h>
#include <cmath>		//std::isfinite std::remainder
#include <cfloat>		//FLT_EPSILON
#include <algorithm>	//std::min std::max
#include "SDK.h"
class CHitscan
{
private:
	Vector GetBestHitbox(CBaseEntity* pEntity, CBaseEntity* pLocal);
	Vector GetBestAngle(CBaseEntity* pEntity, CBaseEntity* pLocal);
	CBaseEntity* GetBestTarget(CBaseEntity* pLocal);
	void LagFix(CBaseEntity* pEntity, Vector& Hitbox, Vector& Out);
public:
	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);

	//bool bEnable = true, bSilentAim = true, DisableInterpolation = true;
	//int iFOV = 30;
};
inline CHitscan gHitscan;