#include "Hitscan.h"
#include "SDK.h"
#include "Utils.h"
#include "bspflags.h"
#include "Rage.h"
#include "PrioritySystem.h"
#include "Vars.h"
#pragma warning(disable : 4715) // SHUSH NIGGER LOL
bool Pos(CBaseEntity* pSkip, CBaseEntity* pEntity, Vector from, Vector to)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT | CONTENTS_GRATE);

	CTraceFilter filter;
	filter.pSkip = pSkip;

	CGameTrace trace;
	i::enginetrace->TraceRay(ray, mask, &filter, &trace);

	return ((trace.m_pEnt && trace.m_pEnt == pEntity) || trace.fraction >= 0.99f);
}
Vector CHitscan::GetBestHitbox(CBaseEntity* pEntity, CBaseEntity* pLocal)
{
	/* i had to modify and do int (buildinghere)hitbox = 0; 
	*	beacuse we are setting a hitbox from the sentry gun, 
	*	then we're trying to get that same hitbox on the player 
	*	but that hitbox doesnt exist on the player 
	*	so we get stuck on aimbot basically. - vannie 
	*/
	int PreferredHitbox = BODY;
	int nigga = pEntity->GetClientClass()->iClassID;
	if (nigga == CObjectSentrygun)
	{
		int sentrygunhitbox = 0;
		CTFObjectSentryGun* pSentryGun = reinterpret_cast<CTFObjectSentryGun*>(pEntity);

		if (!pSentryGun || !pSentryGun->IsAlive())
			return Vector();
		for (int i = sentrygunhitbox; i < pSentryGun->GetNumOfHitboxes(); i++) //we loop through max hitboxes, starting value being our current preferred
		{
			if (!Pos(pLocal, pSentryGun, pLocal->GetShootPos(), pSentryGun->GetHitboxPosition(sentrygunhitbox))) //if the preferred hitbox isn't visible then set it to one of the avaliable. Once avaliable again, since the function runs every tick, it'll get set back automatically.
				sentrygunhitbox = i;
		}
		return pSentryGun->GetHitboxPosition(sentrygunhitbox);
	}
	if (nigga == CObjectDispenser)
	{
		int dispenserhitbox = 0;
		CObjectDispenserClass* pDispenser = reinterpret_cast<CObjectDispenserClass*>(pEntity);

		if (!pDispenser || !pDispenser->IsAlive())
			return Vector();

		for (int i = dispenserhitbox; i < pDispenser->GetNumOfHitboxes(); i++) //we loop through max hitboxes, starting value being our current preferred
		{
			if (!Pos(pLocal, pDispenser, pLocal->GetShootPos(), pDispenser->GetHitboxPosition(dispenserhitbox))) //if the preferred hitbox isn't visible then set it to one of the avaliable. Once avaliable again, since the function runs every tick, it'll get set back automatically.
				dispenserhitbox = i;
		}
		return pDispenser->GetHitboxPosition(dispenserhitbox);
	}
	if (nigga == CTFPlayer)
	{
		if (Vars::Hitscan::MultiPointIfNoHitboxesAreVisible)
		{
			//int nogger;
			for (int i = PreferredHitbox; i < pEntity->GetNumOfHitboxes(); i++) //we loop through max hitboxes, starting value being our current preferred
			{
				if (!Pos(pLocal, pEntity, pLocal->GetShootPos(), pEntity->GetHitboxPosition(PreferredHitbox))) //if the preferred hitbox isn't visible then set it to one of the avaliable. Once avaliable again, since the function runs every tick, it'll get set back automatically.
					PreferredHitbox = i;
			}
		}
		else
		{
			if (Vars::Hitscan::iHitbox == 0)
			{
				if (pLocal->GetClassNum() == TF2_Heavy || pLocal->GetClassNum() == TF2_Scout)
					PreferredHitbox = BODY;
				if (pLocal->GetClassNum() == TF2_Sniper || pLocal->GetClassNum() == TF2_Spy)
					PreferredHitbox = HEAD;

				//if (Pos(pLocal, pEntity, pLocal->GetShootPos(), pEntity->GetHitboxPosition(PreferredHitbox)))
				return pEntity->GetHitboxPosition(PreferredHitbox);
			}
			if (Vars::Hitscan::iHitbox == 1) // HEAD
			{
				//if (Pos(pLocal, pEntity, pLocal->GetShootPos(), pEntity->GetHitboxPosition(HEAD)))
				return pEntity->GetHitboxPosition(HEAD);
			}
			if (Vars::Hitscan::iHitbox == 2) // BODY
			{
				//if (Pos(pLocal, pEntity, pLocal->GetShootPos(), pEntity->GetHitboxPosition(BODY)))
				return pEntity->GetHitboxPosition(BODY);
			}
			if (Vars::Hitscan::iHitbox == 3) // PELVIS
			{
				//if (Pos(pLocal, pEntity, pLocal->GetShootPos(), pEntity->GetHitboxPosition(PELVIS)))
				return pEntity->GetHitboxPosition(PELVIS);
			}
		}
		return pEntity->GetHitboxPosition(PreferredHitbox);
	}
}
#include "Math.h"
Vector CHitscan::GetBestAngle(CBaseEntity* pEntity, CBaseEntity* pLocal)
{
	auto Hitbox = GetBestHitbox(pEntity, pLocal);
	if (Hitbox.IsZero()) return Vector();

	return Math::CalcAngle(pLocal->GetEyePosition(), GetBestHitbox(pEntity, pLocal));
}
#include "Vars.h"

#include "EntityCache.h"
CBaseEntity* CHitscan::GetBestTarget(CBaseEntity* pLocal)
{
	CBaseEntity* BestEntity = nullptr;
	float FOV = FLT_MAX;
	std::vector<std::pair<CBaseEntity*, float>> closest_entities;

	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (pEntity->IsDormant() || !pEntity->IsAlive())
			continue;
		int nIndex = pEntity->GetIndex();
		if (gPriority.ShouldBeIgnored(pEntity))
			continue;

		player_info_t pInfo;

		if (!i::engine->GetPlayerInfo(nIndex, &pInfo))
			continue;

		int userId = pInfo.friendsID;

		Vector viewangles;
		i::engine->GetViewAngles(viewangles);
		/* stolen from seowned :steamhappy: */
		Vector vAngleTo = Math::CalcAngle(pLocal->GetEyePosition(), GetBestHitbox(pEntity, pLocal));
		float GetFOV = Math::CalcFov(viewangles, vAngleTo);
		/*
		if (GetFOV < FOV && GetFOV < Vars::Hitscan::iFov)
		{
			if (gPriority.ShouldBeRage(userId) != "")
				return pEntity;

			FOV = GetFOV;
			BestEntity = pEntity;
		}
		*/
		if (GetFOV < FOV && GetFOV < Vars::Hitscan::iFov) {
			// Add to the closest entities list and sort it
			closest_entities.emplace_back(pEntity, GetFOV);
			std::sort(closest_entities.begin(), closest_entities.end(),
				[](const auto& a, const auto& b) { return a.second < b.second; });

			// Keep only the top two closest entities
			if (closest_entities.size() > Vars::Hitscan::MaxPlayers)
				closest_entities.pop_back();
		}
	}
	// Find the best entity from the two closest
	for (const auto& entity_pair : closest_entities) {
		CBaseEntity* entity = entity_pair.first;
		int userId = entity->GetIndex();
		if (gPriority.ShouldBeRage(userId) != "")
			return entity;
		if (entity_pair.second < FOV) {
			FOV = entity_pair.second;
			BestEntity = entity;
		}
	}
	for (const auto& Building : gEntityCache.GetGroup(EGroupType::BUILDINGS_ENEMIES))
	{
		if (!Building->IsAlive())
			continue;

		Vector viewangles;


		i::engine->GetViewAngles(viewangles);
		/* stolen from seowned :steamhappy: */
		Vector vAngleTo = Math::CalcAngle(pLocal->GetEyePosition(), GetBestHitbox(Building, pLocal));
		float GetFOV = Math::CalcFov(viewangles, vAngleTo);
		if (GetFOV < FOV && GetFOV < Vars::Hitscan::iFov)
		{
			FOV = GetFOV;
			BestEntity = Building;
		}

	}
	return BestEntity;
}

//#include "Projectile.h"
#include "Utils.h"
#include "CMoveSim.h"
#undef max
/*
void CHitscan::LagFix(CBaseEntity* pEntity, Vector& Hitbox, Vector& Out) {
	if (pEntity->GetClientClass()->iClassID != CTFPlayer)
		return;
	if (!gMovementSim.Initialize(pEntity->as<C_BasePlayer>()))
		return;
	// TIME_TO_TICKS(I::GlobalVars-curtime - ent-simulation)
	// server time - entity time
		
		entity time and sever time are nearly THE same
but it updates differently
and by those delta u get said choke
		
	for (int i = 0; i < 24; ++i)
	{
		Vector abs = gMovementSim.RunTick(false);
		int choke = (gInts.Globals->curtime - pEntity->GetSimulationTime());

		if (TIME_TO_TICKS(choke) > i)
			break;
		abs.z = Hitbox.z;
		Hitbox = abs;
		Out = Hitbox;
		break; // break so we restore.
	}
	gMovementSim.Restore();
}
*/

void CHitscan::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	if (!Vars::Hitscan::bEnable || !Utils::IsKeyPressed(Vars::Hitscan::Key))
		return;

	auto pEntity = GetBestTarget(pLocal);
	if (!pEntity || !pLocal || !pLocal->GetActiveWeapon())
		return;
	if (!pLocal->IsAlive())
		return;

	float projectile_speed = 0.0f, projectile_gravity = 0.0f;
	auto is_projectile_weapon = Utils::get_projectile_info(pLocal, projectile_speed, projectile_gravity);

	if (is_projectile_weapon)
		return;

	Vector m_vOldViewAngle = pCmd->viewangles;
	float m_fOldSideMove = pCmd->sidemove;
	float m_fOldForwardMove = pCmd->forwardmove;
	Vector Hitbox = GetBestHitbox(pEntity, pLocal);
	if (Hitbox.IsZero())
		return;
	if (!Pos(pLocal, pEntity, pLocal->GetShootPos(), Hitbox))
		return;
	Vector Final = Math::CalcAngle(pLocal->GetEyePosition(), Hitbox);

	pCmd->viewangles = Final;

	if (!Vars::Hitscan::bSilentAim)
		i::engine->SetViewAngles(pCmd->viewangles);

	if (Vars::Hitscan::bSilentAim)
		Utils::FixMovement(pCmd, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
	if (Vars::Hitscan::bAutoShoot)
	{

		if (Vars::DT::DoubleTap && Vars::DT::Shifted == 21 && Utils::CanPrimaryAttack(pLocal, pLocal->GetActiveWeapon()))
		{
			Vars::DT::shifting = true;
		}
		pCmd->buttons |= IN_ATTACK;

	}
}