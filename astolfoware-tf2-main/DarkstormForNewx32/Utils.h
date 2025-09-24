#pragma once
#include "SDK.h"
#include "Math.h"
#include <cstdio>
#include "bspflags.h"
#include "CreateMove.h"
#include <chrono>
#include <iostream>
#include <random>
#include "Vars.h"
#undef max
namespace Utils
{
	__inline int RandInt(int min, int max)
	{
		//This allows us to reach closer to true randoms generated
		//I don't think we need to update the seed more than once
		static const unsigned nSeed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine gen(nSeed);
		std::uniform_int_distribution<int> distr(min, max);
		return distr(gen);
	}

	inline std::wstring ConvertUtf8ToWide(const std::string_view& str)
	{
		int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), NULL, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), &wstr[0], count);
		return wstr;
	}

	inline void FixMovement(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove)
	{
		float deltaView = pCmd->viewangles.y - m_vOldAngles.y;
		float f1;
		float f2;

		if (m_vOldAngles.y < 0.f)
			f1 = 360.0f + m_vOldAngles.y;
		else
			f1 = m_vOldAngles.y;

		if (pCmd->viewangles.y < 0.0f)
			f2 = 360.0f + pCmd->viewangles.y;
		else
			f2 = pCmd->viewangles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * m_fOldForward + cos(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * m_fOldForward + sin(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
	}
	inline bool get_projectile_info(CBaseEntity* local_player, float& speed, float& gravity) {
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
	inline float get_lerp_time() {
		static auto cl_interp = i::cvars->FindVar("cl_interp");
		static auto cl_interp_ratio = i::cvars->FindVar("cl_interp_ratio");
		static auto cl_updaterate = i::cvars->FindVar("cl_updaterate");
		static auto min_ud_rate = i::cvars->FindVar("sv_minupdaterate");
		static auto max_ud_rate = i::cvars->FindVar("sv_maxupdaterate");
		static auto c_min_ratio = i::cvars->FindVar("sv_client_min_interp_ratio");
		static auto c_max_ratio = i::cvars->FindVar("sv_client_max_interp_ratio");

		int ud_rate = cl_updaterate->GetInt();

		if (min_ud_rate && max_ud_rate)
			ud_rate = max_ud_rate->GetInt();

		float ratio = cl_interp_ratio->GetFloat();

		if (ratio == 0)
			ratio = 1.f;

		float lerp = cl_interp->GetFloat();

		if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
			ratio = std::clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

		return std::max(lerp, (ratio / ud_rate));
	}
	inline void TraceHull(Vector vecStart, Vector vecEnd, Vector vecHullMin, Vector vecHullMax,
		unsigned int nMask, ITraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, vecHullMin, vecHullMax);
		i::enginetrace->TraceRay(ray, nMask, pFilter, pTrace);
	}
	inline void GetProjectileFireSetup(CBaseEntity* pPlayer, const Vector& vViewAngles, Vector vOffset, Vector* vSrc)
	{
		if (i::cvars->FindVar("cl_flipviewmodels")->GetInt())
			vOffset.y *= -1.0f;

		Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
		AngleVectors(vViewAngles, &vecForward, &vecRight, &vecUp);

		*vSrc = pPlayer->GetShootPos() + (vecForward * vOffset.x) + (vecRight * vOffset.y) + (vecUp * vOffset.z);

	}
	inline float ATTRIB_HOOK_FLOAT(float baseValue, const char* searchString, CBaseEntity* ent, void* buffer, bool isGlobalConstString) {
		// 55 8B EC 83 EC ? 8B 0D ? ? ? ? 53 56 57 33 F6 33 FF 89 75 ? 89 7D ? 8B 41 ? 85 C0 74 ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 6A
		static DWORD sig = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 8B 0D ? ? ? ? 53 56 57 33 F6 33 FF 89 75 ? 89 7D ? 8B 41 ? 85 C0 74 ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 6A");
		static auto fn = reinterpret_cast<float(__cdecl*)(float, const char*, CBaseEntity*, void*, bool)>(sig);
		return fn(baseValue, searchString, ent, buffer, isGlobalConstString);
	}
	inline PVOID InitKeyValue()
	{
		typedef PDWORD(__cdecl* Init_t)(int);
		static DWORD dwInitLocation = gSignature.GetClientSignature("E8 ? ? ? ? 83 C4 14 85 C0 74 10 68") + 0x1;
		static DWORD dwInit = ((*(PDWORD)(dwInitLocation)) + dwInitLocation + 4);
		static Init_t InitKeyValues = (Init_t)dwInit;
		return InitKeyValues(32);
	}
	template<typename T>
	inline T UTIL_EntityAs(int entindex)
	{
		IClientEntity* pEntity = GetBaseEntity(entindex);

		return pEntity ? pEntity->as<T>() : nullptr;
	}
	inline bool VisCheckShit(Vector PredictedPositon, CUserCmd* pCmd, CBaseEntity* target)
	{
		Ray_t Ray = {};
		CTraceFilter TraceFilter = {};
		CGameTrace Trace = {};

		TraceFilter.pSkip = target;
		Vector vLocalPos = gLocalPlayer->GetEyePosition();
		Vector vVisCheck = vLocalPos;

		switch (gLocalWeapon->GetWeaponID())
		{
		case TF_WEAPON_ROCKETLAUNCHER:
			//case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		case TF_WEAPON_DIRECTHIT:
			// dragons furry
		case 109:
		case TF_WEAPON_FLAREGUN:
			//case TF_WEAPON_FLAREGUN_REVENGE:
		case TF_WEAPON_RAYGUN_REVENGE:
		case TF_WEAPON_COMPOUND_BOW:
		case TF_WEAPON_SYRINGEGUN_MEDIC:
		{
			if (gLocalWeapon->GetItemDefinitionIndex() != Soldier_m_TheOriginal)
			{
				Vector vecOffset(23.5f, 12.0f, -3.0f);

				if (gLocalPlayer->IsDucked())
					vecOffset.z = 8.0f;

				GetProjectileFireSetup(gLocalPlayer, pCmd->viewangles, vecOffset, &vVisCheck);
			}

			break;
		}

		case TF_WEAPON_GRENADELAUNCHER:
		case TF_WEAPON_PIPEBOMBLAUNCHER:
		case TF_WEAPON_STICKBOMB:
		case TF_WEAPON_STICKY_BALL_LAUNCHER:
		{
			/*

			Vector vecAngle = Vector(), vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
			Math::AngleVectors({ -RAD2DEG(out.m_flPitch), RAD2DEG(out.m_flYaw), 0.0f }, &vecForward, &vecRight, &vecUp);
			Vector vecVelocity = ((vecForward * ProjInfo.m_flVelocity) - (vecUp * 200.0f));
			Math::VectorAngles(vecVelocity, vecAngle);
			out.m_flPitch = -DEG2RAD(vecAngle.x);

			break;
			*/
			// idc
		}
		default: break;
		}
		TraceHull(vVisCheck, PredictedPositon, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

		if (Trace.DidHit())
			return false;
	}
	inline void Trace(Vector& vecStart, Vector& vecEnd, unsigned int nMask, ITraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd);
		i::enginetrace->TraceRay(ray, nMask, pFilter, pTrace);
	}
	inline bool VisPosMask(CBaseEntity* pSkip, const CBaseEntity* pEntity, Vector& from, Vector& to, unsigned int nMask)
	{
		CGameTrace trace = {};
		CTraceFilter filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		return ((trace.m_pEnt && trace.m_pEnt == pEntity) || trace.fraction == 1.f);
	}
	inline bool is_visible(CBaseEntity* skip, CBaseEntity* target, Vector from, Vector to, unsigned int mask) {
		CGameTrace trace;
		CTraceFilter filter;
		filter.pSkip = skip;
		Ray_t ray;
		ray.Init(from, to);
		i::enginetrace->TraceRay(ray, mask, &filter, &trace);

		return ((trace.m_pEnt && trace.m_pEnt == target) || trace.fraction > 0.99f);
	}
	inline bool is_visible(CBaseEntity* skip, CBaseEntity* target, Vector from, Vector to, unsigned int mask, Vector mins, Vector maxs) {
		CGameTrace trace;
		CTraceFilter filter;
		filter.pSkip = skip;
		Ray_t ray;
		ray.Init(from, to, mins, maxs);
		i::enginetrace->TraceRay(ray, mask, &filter, &trace);

		return ((trace.m_pEnt && trace.m_pEnt == target) || trace.fraction > 0.99f);
	}
	inline bool IsKeyPressed(int i)
	{
		// "None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "R", "CTRL", "F"
		switch (i)
		{
		case 0: //None
			return true;
		case 1: //Mouse 1
			return GetAsyncKeyState(VK_LBUTTON);
		case 2:
			return GetAsyncKeyState(VK_RBUTTON);
		case 3:										  //Mouses 1-5
			return GetAsyncKeyState(VK_MBUTTON);
		case 4:
			return GetAsyncKeyState(VK_XBUTTON1);
		case 5:
			return GetAsyncKeyState(VK_XBUTTON2);
		case 6: //Shift
			return GetAsyncKeyState(VK_SHIFT);
		case 7: //Alt
			return GetAsyncKeyState(VK_MENU);
		case 8: //R
			return GetAsyncKeyState(0x52);
		case 9: // CTRL
			return GetAsyncKeyState(VK_CONTROL);
		case 10: //F
			return GetAsyncKeyState(0x46);
		default:
			return false;
		}

		return false;
	}
	inline bool candodt() {
		if (!i::clientstate || !gLocalPlayer)
			return false;
		static auto sv_maxusrcmdprocessticks = i::cvars->FindVar("sv_maxusrcmdprocessticks");
		const int iMaxTicks = sv_maxusrcmdprocessticks ? sv_maxusrcmdprocessticks->GetInt() : 22;

		return i::clientstate->chokedcommands < 22 && Vars::DT::Shifted + i::clientstate->chokedcommands < iMaxTicks;
	}
	inline bool CanPrimaryAttack(CBaseEntity* pOwner, CBaseCombatWeapon* pWeapon)
	{
		bool bOut = false;
		if (!pWeapon)
			return false;
		if (!pOwner)
			return false;
		if (pOwner) {
			float flCurTime = static_cast<float>(pOwner->GetTickBase()) * i::globals->interval_per_tick;
			bOut = pWeapon->m_flNextPrimaryAttack() <= flCurTime && pOwner->m_flNextAttack() <= flCurTime;
		}

		return bOut;
	}
	inline bool IsPlayerOnSteamFriendList(CBaseEntity* pPlayer)
	{
		player_info_t pi = { };

		if (i::engine->GetPlayerInfo(pPlayer->GetIndex(), &pi) && pi.friendsID)
		{
			CSteamID steamID{ pi.friendsID, 1, k_EUniversePublic, k_EAccountTypeIndividual };
			return i::steamfriends002->HasFriend(steamID, k_EFriendFlagImmediate);
		}

		return false;
	}
}