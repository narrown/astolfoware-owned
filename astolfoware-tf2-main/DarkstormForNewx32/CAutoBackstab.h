#pragma once
#include "SDK.h"

class CAutoBackstab {
private:
	// Stab::CanBackstab(const Vec3 &vSrc, const Vec3 &vDst, Vec3 vWSCDelta)
	bool CanBackstab(const Vector& vSrc, const Vector& vDst, Vector vWSCDelta);
	CBaseEntity* GetPlayers(CBaseEntity* pLocal, CUserCmd* pCmd);
	bool TraceMelee(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, const Vector& vViewAngles, CBaseEntity** pEntityOut);
	bool IsValid(CBaseEntity* pEntity);
	
public:
	void Run(CUserCmd* pCmd);
};

inline CAutoBackstab gAutoBackstab;