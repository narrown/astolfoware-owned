#pragma once
#include "SDK.h"
#include "Math.h"
#include <optional>
#include "CreateMove.h"
#include "Utils.h"
#define CHECK_VALID( _v ) 0

#define FastSqrt(x)	(sqrt)(x)

#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_2      (M_PI * 2.f)

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#define M_RADPI		57.295779513082

#define PI			3.14159265358979323846

#define M_PHI		1.61803398874989484820 // golden ratio
struct Shit
{
	float m_flPitch;
	float m_flYaw;
	float m_flTime;
};
class CProjectile
{
public:
	Vector FinalPositionForVisual;
	Vector AbsOriginForThing;
	Vector TargetCenter;
	Vector FinalPositionLol;
	bool Success;
	CBaseEntity* TargetVisual;
	Shit Ass = {  };
	CBaseEntity* get_closest_to_crosshair(CBaseEntity* local_player, const Vector view_angles, int settings_fov);
	bool is_position_visible(CBaseEntity* local_player, Vector& source, CBaseEntity* pEntity);
	Vector GetHitBoxPos(CBaseEntity* pLocal, CBaseEntity* pEntity, Vector AbsOrigin);
	Vector SentryHitBox(CBaseEntity* pEntity, CBaseEntity* local_player);
	void CompensateForPing(CBaseEntity* local_player, CBaseEntity* target, float projectile_speed, float projectile_gravity, const int hitbox, CUserCmd* pCmd);
	void Run(CBaseEntity* local_player, CUserCmd* cmd);
	Vector calc_angle(const Vector& source, const Vector& destination, const Vector& view_angles) {
		Vector delta = source - destination;
		auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
		Vector angles;
		angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - view_angles.x;
		angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - view_angles.y;
		angles.z = 0.0f;

		if (delta.x >= 0.0)
			angles.y += 180.0f;

		// normalize
		angles.x = std::isfinite(angles.x) ? std::remainderf(angles.x, 360.0f) : 0.0f;
		angles.y = std::isfinite(angles.y) ? std::remainderf(angles.y, 360.0f) : 0.0f;
		angles.z = 0.0f;

		return angles;
	}
	Vector calc_angle_projectile(const Vector& source, const Vector& destination) {
		Vector angles = Vector(0.0f, 0.0f, 0.0f);
		Vector delta = (source - destination);
		float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * M_RADPI);
		angles.y = (atanf(delta.y / delta.x) * M_RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}
	bool get_projectile_info(CBaseEntity* local_player, float& speed, float& gravity) {
		if (!local_player)
			return false;
		if (!local_player->GetActiveWeapon())
			return false;
		switch (local_player->GetActiveWeapon()->GetItemDefinitionIndex())
		{
		case Soldier_m_RocketLauncher:
		case Soldier_m_RocketLauncherR:
		case Soldier_m_TheBlackBox:
		case Soldier_m_TheCowMangler5000:
		case Soldier_m_TheOriginal:
		case Soldier_m_FestiveRocketLauncher:
		case Soldier_m_TheBeggarsBazooka:
		case Soldier_m_SilverBotkillerRocketLauncherMkI:
		case Soldier_m_GoldBotkillerRocketLauncherMkI:
		case Soldier_m_RustBotkillerRocketLauncherMkI:
		case Soldier_m_BloodBotkillerRocketLauncherMkI:
		case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
		case Soldier_m_DiamondBotkillerRocketLauncherMkI:
		case Soldier_m_SilverBotkillerRocketLauncherMkII:
		case Soldier_m_GoldBotkillerRocketLauncherMkII:
		case Soldier_m_FestiveBlackBox:
		case Soldier_m_TheAirStrike:
		case Soldier_m_WoodlandWarrior:
		case Soldier_m_SandCannon:
		case Soldier_m_AmericanPastoral:
		case Soldier_m_SmalltownBringdown:
		case Soldier_m_ShellShocker:
		case Soldier_m_AquaMarine:
		case Soldier_m_Autumn:
		case Soldier_m_BlueMew:
		case Soldier_m_BrainCandy:
		case Soldier_m_CoffinNail:
		case Soldier_m_HighRollers:
		case Soldier_m_Warhawk: {
			speed = 1100.0f;
			gravity = 0.0f;
			break;
		}
		case Soldier_m_TheDirectHit: {
			speed = 1980.0f;
			gravity = 0.0f;
			break;
		}

		case Soldier_m_TheLibertyLauncher: {
			speed = 1540.0f;
			gravity = 0.0f;
			break;
		}
		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
		case Demoman_m_TheIronBomber:
		case Demoman_m_Autumn:
		case Demoman_m_MacabreWeb:
		case Demoman_m_Rainbow:
		case Demoman_m_SweetDreams:
		case Demoman_m_CoffinNail:
		case Demoman_m_TopShelf:
		case Demoman_m_Warhawk:
		case Demoman_m_ButcherBird: {
			speed = 1217.0f;
			gravity = 0.4f;
			break;
		}
		case Soldier_s_TheRighteousBison:
		case Engi_m_ThePomson6000: {
			speed = 1200.0f;
			gravity = 0.0f;
			break;
		}
		case Demoman_m_TheLooseCannon: {
			speed = 1453.9f;
			gravity = 0.4f;
			break;
		}
		case Demoman_m_TheLochnLoad: {
			speed = 1513.3f;
			gravity = 0.4f;
			break;
		}
		case Engi_m_TheRescueRanger:
		case Medic_m_FestiveCrusadersCrossbow:
		case Medic_m_CrusadersCrossbow: {
			speed = 2400.0f;
			gravity = 0.2f;
			break;
		}
		case Pyro_m_DragonsFury: {
			speed = 3000.0f;
			gravity = 0.0f;
			break;
		}
		case Pyro_m_FlameThrower:
		case Pyro_m_ThePhlogistinator:
		case Pyro_m_TheBackburner:
		case Pyro_m_TheDegreaser: {
			speed = 1000.0f;
			gravity = 0.0f;
			break;
		}
		case Pyro_s_TheDetonator:
		case Pyro_s_TheFlareGun:
		case Pyro_s_FestiveFlareGun:
		case Pyro_s_TheScorchShot: {
			speed = 2000.0f;
			gravity = 0.3f;
			break;
		}
		case Pyro_s_TheManmelter: {
			speed = 3000.0f;
			gravity = 0.2f;
			break;
		}
		case Medic_m_SyringeGun:
		case Medic_m_SyringeGunR:
		case Medic_m_TheBlutsauger:
		case Medic_m_TheOverdose: {
			speed = 1000.0f;
			gravity = 0.2f;
			break;
		}
		case Sniper_m_TheHuntsman:
		case Sniper_m_FestiveHuntsman:
		case Sniper_m_TheFortifiedCompound: {
			float charge = (i::globals->curtime - local_player->GetActiveWeapon()->GetChargeBeginTime());
			speed = { ((fminf(fmaxf(charge, 0.0f), 1.0f) * 800.0f) + 1800.0f) };
			gravity = { ((fminf(fmaxf(charge, 0.0f), 1.0f) * -0.4f) + 0.5f) };
			break;
		}
		case Demoman_s_StickybombLauncher:
		case Demoman_s_StickybombLauncherR:
		case Demoman_s_FestiveStickybombLauncher:
		case Demoman_s_TheScottishResistance:
		{
			//Probably wrong
			const float charge = (i::globals->curtime - local_player->GetActiveWeapon()->GetChargeBeginTime());
			speed = Math::RemapValClamped(charge, 0.0f, 4.f, 900, 2400);
			gravity = Math::RemapValClamped(charge, 0.0f, 4.f, 0.5f, 0.0f);
			break;
		}
		case Demoman_s_TheQuickiebombLauncher:
		{
			//Probably wrong
			const float charge = (i::globals->curtime - local_player->GetActiveWeapon()->GetChargeBeginTime());
			//out = {
			speed = Math::RemapValClamped(charge, 0.0f, 1.2f, 900, 2400);
			gravity = Math::RemapValClamped(charge, 0.0f, 1.2f, 0.5f, 0.0f);

			break;
		}
		default:
			speed = 0.0f;
			gravity = 0.0f;
			return false;
		}
		return true;
	}
	void DrawPredictedPath(CBaseEntity* local_player, CBaseEntity* target, float projectile_speed, const int hitbox);
	Vector Extrapolate(float time, CBaseEntity* pEntity, Vector m_vPosition, Vector m_vVelocity, Vector m_vAcceleration, Vector AbsOrigin);
	inline float solve_projectile_speed(CBaseCombatWeapon* weapon, const Vector& a, const Vector& b, float projectile_speed, float projectile_gravity) {
		if (weapon->GetWeaponID() != TF_WEAPON_GRENADELAUNCHER && weapon->GetWeaponID() != TF_WEAPON_PIPEBOMBLAUNCHER)
			return  projectile_speed;

		float flVelocity = projectile_speed;
		const float flGrav = projectile_gravity * 800.0f;
		const Vector vDelta = a - b;
		const float flDist = vDelta.Length2D();

		const float flRoot = pow(flVelocity, 4) - flGrav * (flGrav * pow(flDist, 2) + 2.f * vDelta.z * pow(flVelocity, 2));
		if (flRoot < 0.f)
			return 0;

		const float flPitch = atan((pow(flVelocity, 2) - sqrt(flRoot)) / (flGrav * flDist));
		const float flTime = flDist / (cos(flPitch) * flVelocity);

		float flDrag = 0.f;
		float flMaxVelocity = 2000;


		switch (weapon->GetItemDefinitionIndex()) // the remaps are dumb but they work so /shrug
		{
		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
		case Demoman_m_Autumn:
		case Demoman_m_MacabreWeb:
		case Demoman_m_Rainbow:
		case Demoman_m_SweetDreams:
		case Demoman_m_CoffinNail:
		case Demoman_m_TopShelf:
		case Demoman_m_Warhawk:
		case Demoman_m_ButcherBird:
		case Demoman_m_TheIronBomber: flDrag = Math::RemapValClamped(flVelocity, 1217.f, flMaxVelocity, 0.120f, 0.200f); break; // 0.120 normal, 0.200 capped, 0.300 v3000
		case Demoman_m_TheLochnLoad: flDrag = Math::RemapValClamped(flVelocity, 1504.f, flMaxVelocity, 0.070f, 0.085f); break; // 0.070 normal, 0.085 capped, 0.120 v3000
		case Demoman_m_TheLooseCannon: flDrag = Math::RemapValClamped(flVelocity, 1454.f, flMaxVelocity, 0.385f, 0.530f); break; // 0.385 normal, 0.530 capped, 0.790 v3000
		case Demoman_s_StickybombLauncher:
		case Demoman_s_StickybombLauncherR:
		case Demoman_s_FestiveStickybombLauncher:
		case Demoman_s_TheQuickiebombLauncher:
		case Demoman_s_TheScottishResistance: flDrag = Math::RemapValClamped(flVelocity, 922.f, flMaxVelocity, 0.085f, 0.190f); break; // 0.085 low, 0.190 capped, 0.230 v2400
		}


		return flVelocity = flVelocity - flVelocity * flTime * flDrag;
	}
	float get_latency()
	{
		INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(i::engine->GetNetChannelInfo());
		if (pNetChannel)
		{
			float fLatency = pNetChannel->GetLatency(0) + pNetChannel->GetLatency(1) + Utils::get_lerp_time(); // add lerp time 
			int nCmdRate = 66;
			fLatency -= (0.5f / nCmdRate) + TICKS_TO_TIME(1.0f);
			fLatency -= TICKS_TO_TIME(0.5f);

			return fLatency;
		}
		return 0.0f;
	}
	inline bool CalcProjAngle(const Vector& vLocalPos, const Vector& vTargetPos, float projectile_speed, float projectile_gravity, Shit& Shit)
	{
		const float fGravity = i::cvars->FindVar("sv_gravity")->GetFloat() * projectile_gravity;
		const Vector vDelta = vTargetPos - vLocalPos;
		const float fHyp = sqrt(vDelta.x * vDelta.x + vDelta.y * vDelta.y);
		const float fDist = vDelta.z;
		const float fVel = solve_projectile_speed(gLocalPlayer->GetActiveWeapon(), vLocalPos, vTargetPos, projectile_speed, projectile_gravity);
		if (!fGravity)
		{
			const Vector vAngleTo = Math::CalcAngle(vLocalPos, vTargetPos);
			Shit.m_flPitch = -DEG2RAD(vAngleTo.x);
			Shit.m_flYaw = DEG2RAD(vAngleTo.y);
		}
		else
		{	//	arch
			const float fRoot = pow(fVel, 4) - fGravity * (fGravity * pow(fHyp, 2) + 2.f * fDist * pow(fVel, 2));
			if (fRoot < 0.f)
			{
				return false;
			}
			Shit.m_flPitch = atan((pow(fVel, 2) - sqrt(fRoot)) / (fGravity * fHyp));
			Shit.m_flYaw = atan2(vDelta.y, vDelta.x);
		}
		float m_flTime = fHyp / (cos(Shit.m_flPitch) * fVel);
		Shit.m_flTime = m_flTime += get_latency();
		return true;
	}
};

inline CProjectile gProjectileAim;