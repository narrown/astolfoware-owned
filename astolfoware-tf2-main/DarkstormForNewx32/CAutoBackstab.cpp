#include "CAutoBackstab.h"
#include "EntityCache.h"
#include "Utils.h"

bool CAutoBackstab::CanBackstab(const Vector& vSrc, const Vector& vDst, Vector vWSCDelta) {
	vWSCDelta.z = 0;
	vWSCDelta.NormalizeInPlace();

	Vector vecEyeSpy;
	AngleVectors(vSrc, &vecEyeSpy);
	vecEyeSpy.z = 0;
	vecEyeSpy.NormalizeInPlace();

	Vector vecEyeVictim;
	AngleVectors(vDst, &vecEyeVictim);
	vecEyeVictim.z = 0;
	vecEyeVictim.NormalizeInPlace();

	if (vWSCDelta.Dot(vecEyeVictim) <= 0.01f)
		return false;

	if (vWSCDelta.Dot(vecEyeSpy) <= 0.5f)
		return false;

	if (vecEyeSpy.Dot(vecEyeVictim) <= -0.3f)
		return false;

	return true;
}
bool CAutoBackstab::IsValid(CBaseEntity* pEntity) {
	if (!pEntity)
		return false;
	if (pEntity->IsDormant())
		return false;
	if (pEntity->GetClientClass()->iClassID != CTFPlayer)
		return false;
	if (!pEntity->IsAlive())
		return false;
	if (pEntity->GetTeamNum() == gLocalPlayer->GetTeamNum())
		return false;

	return true;
}
// marge..
// do we even need this? dont we just need to check if we can backstab or not?
// idk pastarino go paste or something
// -vannie
bool CAutoBackstab::TraceMelee(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, const Vector& vViewAngles, CBaseEntity** pEntityOut)
{
	float flRange = (48.0f * 0.8f);

	if (flRange <= 0.0f)
		return false;

	Vector vForward;
	AngleVectors(vViewAngles, &vForward);
	Vector vTraceStart = pLocal->GetShootPos();
	Vector vTraceEnd = (vTraceStart + (vForward * flRange));

	CGameTrace Trace = {};
	CTraceFilter Filter = {};
	Filter.pSkip = pLocal;
	Utils::TraceHull(vTraceStart, vTraceEnd, { -18.0f, -18.0f, -18.0f }, { 18.0f, 18.0f, 18.0f }, MASK_SOLID, &Filter, &Trace);

	if (IsValid(Trace.m_pEnt))
	{
		if (pEntityOut && !*pEntityOut)
			*pEntityOut = Trace.m_pEnt;

		return true;
	}

	return false;
}


CBaseEntity* CAutoBackstab::GetPlayers(CBaseEntity* pLocal, CUserCmd* pCmd) {

	CBaseEntity* pTrace = nullptr;
	CBaseEntity* Return = nullptr;

	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES)) {
		Vector vAngleTo = Math::CalcAngle(pLocal->GetShootPos(), pEntity->GetHitboxPosition(BODY));

		if (!TraceMelee(pLocal, gLocalWeapon, vAngleTo, &pTrace) || pTrace != pEntity)
			continue;

		Vector WSC;
		Vector LocalWSC;
		pEntity->GetWorldSpaceCenter(WSC);
		gLocalPlayer->GetWorldSpaceCenter(LocalWSC);

		if (!CanBackstab(vAngleTo, pEntity->GetEyeAngles(), (WSC - LocalWSC)))
			continue;

		Return = pEntity;
		/*
		pCmd->viewangles = vAngleTo;
		pCmd->buttons |= IN_ATTACK;

		break;
		*/ // i do not TRUST doing this here
	}

	return Return;
}
#include "Vars.h"
void CAutoBackstab::Run(CUserCmd* pCmd) {
	if (!Vars::Misc::AutoBackstab)
		return;
	if (!gLocalPlayer || !gLocalPlayer->GetActiveWeapon() || !gLocalWeapon)
		return;
	if (gLocalPlayer->GetClassNum() != TF2_Spy) // dont even try if we are not a spy
		return;
	if (gLocalWeapon->GetWeaponID() != TF_WEAPON_KNIFE) // make sure its the spy's knife
		return; 
	
	CBaseEntity* Nigger = GetPlayers(gLocalPlayer, pCmd);
	if (!Nigger || !Nigger->IsAlive() || Nigger->IsDormant())
		return;
	Vector vAngleTo = Math::CalcAngle(gLocalPlayer->GetShootPos(), Nigger->GetHitboxPosition(BODY));

	Vector WSC;
	Vector LocalWSC;
	Nigger->GetWorldSpaceCenter(WSC);
	gLocalPlayer->GetWorldSpaceCenter(LocalWSC);

	if (!CanBackstab(vAngleTo, Nigger->GetEyeAngles(), (WSC - LocalWSC)))
		return;
	pCmd->viewangles = vAngleTo;
	pCmd->buttons |= IN_ATTACK;
}