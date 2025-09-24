#include "Projectile.h"
#include "Utils.h"
#include "bspflags.h"
#include "Math.h"
#include "Vector2.h"
#include "CMoveSim.h"
#include "Vars.h"
#include "EntityCache.h"
#include "PrioritySystem.h"
#pragma warning(disable : 4018) // SHUSH NIGGER LOL
#pragma warning(disable : 4715) // SHUSH NIGGER LOL
//#include "PrioritySystem.h"

/*
* CHATGPT KINGS $$$$$$$$$$$$$$$$$$$$$$$
* i chatgpt'd some stuff beacuse im a lazy tranny-ACK! -Vannie
*
* Originally made by OverflowTF2's maker (i dont remember the name)
* I just improved it by actually predicting shit, and movesim ofc, and "proper" vischeck
* (i still have some issues due to shootpos not being the projectile weapon's shoot pos but whatev)
*/
std::vector<Vector> m_TargetPath = {};
float DistanceToGround(CBaseEntity* pEntity, Vector AbsOrigin)
{
	if (pEntity->GetFlags() & FL_ONGROUND) return 0.0f;
	auto distance_to_ground = [&pEntity](Vector origin) -> float
	{
		trace_t ground_trace; Ray_t ray;
		CTraceFilter filter; filter.pSkip = pEntity;
		Vector endpos = origin;

		endpos.z -= 8192;
		ray.Init(origin, endpos);
		i::enginetrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
		return 8192.0f * ground_trace.fraction;
	};

	Vector origin = AbsOrigin;
	float v1 = distance_to_ground(origin + Vector(10.0f, 10.0f, 0.0f));
	float v2 = distance_to_ground(origin + Vector(-10.0f, 10.0f, 0.0f));
	float v3 = distance_to_ground(origin + Vector(10.0f, -10.0f, 0.0f));
	float v4 = distance_to_ground(origin + Vector(-10.0f, -10.0f, 0.0f));
	return min(v1, min(v2, min(v3, v4)));
}
#include "CreateMove.h"
Vector CProjectile::Extrapolate(float time, CBaseEntity* pEntity, Vector m_vPosition, Vector m_vVelocity, Vector m_vAcceleration, Vector AbsOrigin)
{
	if (!time)
		return pEntity->GetAbsOrigin();

	static ConVar* sv_gravity = i::cvars->FindVar("sv_gravity");

	Vector startPos = m_vPosition;
	float zdrop;
	if (pEntity->GetFlags() & FL_ONGROUND) {
		return (m_vPosition + (m_vVelocity * time));
	}
	else {
		zdrop = 0.5 * -sv_gravity->GetInt() * pow(time, 2) + m_vVelocity.z * time;
	}

	Vector result(
		startPos.x + (m_vVelocity.x * time),
		startPos.y + (m_vVelocity.y * time),
		startPos.z + zdrop);

	// Calculate distance to the ground
	float distance_to_ground = DistanceToGround(pEntity, AbsOrigin);

	// Adjust the Z-coordinate if below ground level
	if (result.z < startPos.z - distance_to_ground) {
		result.z = startPos.z - distance_to_ground;
	}

	float angleY = 0;
	Math::RotateVec2(*(Vector2*)&result, *(Vector2*)&startPos, DEG2RAD(angleY * (result - startPos).Length2D()));

	return result;
}

#include <algorithm> // For std::sort
#include "PrioritySystem.h"
#include "EntityCache.h"
CBaseEntity* CProjectile::get_closest_to_crosshair(CBaseEntity* local_player, const Vector view_angles, int settings_fov)
{
	CBaseEntity* best_entity = nullptr;
	float max_delta = FLT_MAX;

	std::vector<std::pair<CBaseEntity*, float>> closest_entities;

	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES)) {

		if (!pEntity || pEntity->GetLifeState() != LIFE_ALIVE || pEntity->IsDormant())
			continue;

		if (pEntity->GetTeamNum() == local_player->GetTeamNum())
			continue;

		if (gPriority.ShouldBeIgnored(pEntity))
			continue;

		player_info_t pInfo;
		if (!i::engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
			continue;

		int userId = pInfo.friendsID;
		Vector viewangles;

		i::engine->GetViewAngles(viewangles);
		//float delta = calc_angle(local_player->GetEyePosition(), pEntity->GetEyePosition(), view_angles).length();
		Vector vAngleTo = Math::CalcAngle(local_player->GetEyePosition(), GetHitBoxPos(local_player, pEntity, pEntity->GetAbsOrigin()));
		float GetFOV = Math::CalcFov(viewangles, vAngleTo);

		if (GetFOV < max_delta && GetFOV < Vars::ProjectileAim::AimFOV) {
			// Add to the closest entities list and sort it
			closest_entities.emplace_back(pEntity, GetFOV);
			std::sort(closest_entities.begin(), closest_entities.end(),
				[](const auto& a, const auto& b) { return a.second < b.second; });

			// Keep only the top two closest entities
			if (closest_entities.size() > Vars::ProjectileAim::MaxPlayers)
				closest_entities.pop_back();
		}
	}

	// Find the best entity from the two closest
	for (const auto& entity_pair : closest_entities) {
		CBaseEntity* entity = entity_pair.first;
		int userId = entity->GetIndex();
		if (gPriority.ShouldBeRage(userId) != "")
			return entity;
		if (entity_pair.second < max_delta) {
			max_delta = entity_pair.second;
			best_entity = entity;
		}
	}
	for (const auto& Building : gEntityCache.GetGroup(EGroupType::BUILDINGS_ENEMIES))
	{
		if (!Building->IsAlive())
			continue;

		Vector viewangles;
		i::engine->GetViewAngles(viewangles);
		/* stolen from seowned :steamhappy: */
		//if (!is_position_visible(local_player, WSC, Building))
		//	continue;
		Vector vAngleTo = Math::CalcAngle(local_player->GetEyePosition(), SentryHitBox(Building, local_player));
		float GetFOV = Math::CalcFov(viewangles, vAngleTo);

		if (GetFOV < max_delta && GetFOV < Vars::ProjectileAim::AimFOV) {
			max_delta = GetFOV;
			best_entity = Building;
		}

	}

	return best_entity;
}


bool CProjectile::is_position_visible(CBaseEntity* local_player, Vector& source, CBaseEntity* pEntity)
{
	Ray_t ray;
	Vector local_eye = local_player->GetEyePosition();
	ray.Init(local_eye, source);

	const uint32_t mask = (MASK_SOLID);

	CTraceFilter filter;
	filter.pSkip = local_player;

	CGameTrace trace;
	i::enginetrace->TraceRay(ray, mask, &filter, &trace);

	return ((trace.m_pEnt && trace.m_pEnt == pEntity) || trace.fraction >= 0.99f);
}


#undef max
#undef min 
#define MAX_FLOWS 2



Vector GetWorldSpaceCenterProjectile(Vector AbsOrigin, CBaseEntity* pEntity)
{
	Vector vWorldSpaceCenter;
	Vector vMin, vMax;
	pEntity->GetRenderBounds(vMin, vMax);
	vWorldSpaceCenter = AbsOrigin;
	vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	return vWorldSpaceCenter;
}
Vector CProjectile::GetHitBoxPos(CBaseEntity* pLocal, CBaseEntity* pEntity, Vector AbsOrigin)
{
	Vector WorldSpaceCenter = GetWorldSpaceCenterProjectile(AbsOrigin, pEntity);
	//pEntity->GetWorldSpaceCenter(WorldSpaceCenter);
	if (pLocal->GetClassNum() == TF2_Soldier) /* todo: if the guy is bhopping just shoot the body*/
	{
		return WorldSpaceCenter - Vector(0.0f, 0.0f, 27.0f);
	}
	if (pLocal->GetClassNum() == TF2_Demoman || pLocal->GetClassNum() == TF2_Medic || pLocal->GetClassNum() == TF2_Pyro)
	{
		return (WorldSpaceCenter); /* return center (body/pelvis basically) */
	}
	if (pLocal->GetClassNum() == TF2_Sniper) 
	{
		//return (pEntity->GetHitboxPosition(HEAD));
		/*
		Vector vPos = pEntity->GetHitboxPosition(HEAD);
		if (pLocal->GetActiveWeapon()->GetItemDefinitionIndex() == Sniper_m_TheHuntsman || pLocal->GetClassNum() == TF2_Sniper)
		{
			Vector vEntForward;
			AngleVectors(pEntity->GetEyeAngles(), &vEntForward);
			Vector vToEnt = AbsOrigin - pLocal->GetAbsOrigin();
			vToEnt.NormalizeInPlace();

			if (vToEnt.Dot(vEntForward) > 0.1071f) {
				vPos.z += 5.0f;
			}
		}
		return vPos;
		*/ // fuck off nigger.
		return pEntity->GetHitboxPosition(HEAD);
	}
}
Vector CProjectile::SentryHitBox(CBaseEntity* pEntity, CBaseEntity* local_player) { // todo: use actual hitboxes you braindead nigger
	Vector WSC;
	pEntity->GetWorldSpaceCenter(WSC);
	return WSC + Vector(0, 0, 10);
}
#include "CConsole.h"
/*
*	14/01/2025
*	Fix Overpredicting.
*	Primarely, its doing this beacuse
*   of our lag compensation +'ing too much to time_to_target.
*	Solution: Fix Lag Comp to Not overpredict o algo.
*	6/02/2025 Vannie:
*	Nigger this is still over predicting most of the time but its better now i guess i dotn knýow brah
*/

void CProjectile::CompensateForPing(CBaseEntity* local_player, CBaseEntity* target, float projectile_speed, float projectile_gravity, const int hitbox, CUserCmd* pCmd)
{
	if (target->GetClientClass()->iClassID == CObjectDispenser || target->GetClientClass()->iClassID == CObjectSentrygun) {
		for (int n = 0; n < TIME_TO_TICKS(10.0f); ++n)
		{
			Vector WSC = SentryHitBox(target, local_player);

			FinalPositionLol = WSC;
			FinalPositionForVisual = FinalPositionLol;

			if (Utils::VisCheckShit(WSC, pCmd, target)
				|| !is_position_visible(local_player, WSC, target))
				break;
			break;
		}

	}
	if (target->IsPlayer()) 
	{
		if (!gMovementSim.Initialize(target->as<C_BasePlayer>()))
			return;
		//for (int n = 0; n < TIME_TO_TICKS(maxTime); ++n)
		for (int n = 0; n < TIME_TO_TICKS(10.0f); ++n)
		{
			gMovementSim.RunTick();
			Vector AbsOrigin = gMovementSim.GetOrigin();
			Vector TargetHitbox = GetHitBoxPos(gLocalPlayer, target, AbsOrigin);
			//(Math::GetDistance(gLocalPlayer->GetEyePosition(), AbsOrigin) / projectile_speed); 
		   //Ass.m_flTime += get_latency();

			if (!CalcProjAngle(gLocalPlayer->GetEyePosition(), AbsOrigin, projectile_speed, projectile_gravity, Ass))
				break;


			//Ass.m_flTime += get_latency();
			//auto time = Ass.m_flTime;
			//float fLatencyCompensation = (get_latency() + fPredTime); // remove if it overpredicts again (fPredTime)
			Vector Velocity = target->GetVelocity();
			Vector vAcceleration = Vector(0.0f, 0.0f, i::cvars->FindVar("sv_gravity")->GetFloat() * ((target->GetCondEx2() & TFCondEx2_Parachute) ? 0.448f : 1.0f));
			Vector Predicted = Extrapolate(Ass.m_flTime, target, TargetHitbox, Velocity, vAcceleration, AbsOrigin);

			FinalPositionLol = Predicted;
			FinalPositionForVisual = FinalPositionLol;
			AbsOriginForThing = AbsOrigin;
			TargetCenter = TargetHitbox;
			if (Utils::VisCheckShit(TargetHitbox, pCmd, target)
				|| !is_position_visible(local_player, Predicted, target))
				break;
			break; // break out of the loop so we restore
		}
		gMovementSim.Restore();
	}
	
}
#include "Utils.h"
void CProjectile::Run(CBaseEntity* local_player, CUserCmd* cmd)
{
	if (!local_player->GetActiveWeapon())
		return;
	
	Shit Ass = {  };
	float projectile_speed = 0.0f, projectile_gravity = 0.0f;
	if (!get_projectile_info(local_player, projectile_speed, projectile_gravity))
		return;

	if (!Vars::ProjectileAim::Enable)
		return;
	if (!Utils::IsKeyPressed(Vars::ProjectileAim::Key))
		return;
	
	auto target = get_closest_to_crosshair(local_player, cmd->viewangles, Vars::ProjectileAim::AimFOV);
	if (!target)
		return;
	TargetVisual = target;

	
	Vector m_vOldViewAngle = cmd->viewangles;
	float m_fOldSideMove = cmd->sidemove;
	float m_fOldForwardMove = cmd->forwardmove;

	CompensateForPing(local_player, target, projectile_speed, projectile_gravity, 0, cmd);
	Vector hitbox = GetHitBoxPos(local_player, target, target->GetAbsOrigin());
	if (!is_position_visible(local_player, hitbox, target))
		return;
	if (Utils::VisCheckShit(hitbox, cmd, target)
		|| !is_position_visible(local_player, hitbox, target))
		return;

	if (!is_position_visible(local_player, FinalPositionLol, target))
		return;
	//Vector(-RAD2DEG(Ass.m_flPitch), RAD2DEG(Ass.m_flYaw), 0.0f);
	auto angle = calc_angle_projectile(local_player->GetEyePosition(), FinalPositionLol); // same as other shit
	ClampAngle(angle);
	cmd->viewangles = angle;
	
	if (Vars::ProjectileAim::AutoShoot)
	{
		cmd->buttons |= IN_ATTACK;
	}
	/* Projectile shoot fix thing idk*/
	if (local_player->GetActiveWeapon()->GetItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka && local_player->GetActiveWeapon()->Clip1() > 0)
		cmd->buttons &= ~IN_ATTACK;
	if ((local_player->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER || local_player->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_COMPOUND_BOW) &&
		local_player->GetActiveWeapon()->GetChargeBeginTime() > 0.0f)
		cmd->buttons &= ~IN_ATTACK;

	if (!Vars::ProjectileAim::Silent)
	{
		i::engine->SetViewAngles(cmd->viewangles);
	}
	if (Vars::ProjectileAim::Silent)
	{
		Utils::FixMovement(cmd, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
	}
}