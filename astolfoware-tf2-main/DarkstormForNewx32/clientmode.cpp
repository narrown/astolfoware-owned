#include "clientmode.h"
#include "CreateMove.h"
#include "EnginePred.h"
#include "Vars.h"
#include "Misc.h"
#include "Hitscan.h"
#include "CritHack.h"
#include "Projectile.h"
#include "CritHack.h"
#include "AntiAim.h"
#include "Fakelag.h"
#include "CAutoDet.h"
#include "CMoveSim.h"
#include "EntityCache.h"
#include "Utils.h"
#include "Visuals.h"
#include "MinHook/MinHook.h"
#include "Chams.h"
#include "CAutoBackstab.h"
using namespace clientmode_hooks;

#define DT_WAIT_CALLS 26



bool __fastcall create_move::detour(void* ecx, void* edx, float input_sample_frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number || !pCmd)
		return original(ecx, edx, input_sample_frametime, pCmd);

	CBaseEntity* pBaseEntity = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<CBaseEntity>();
	if (pBaseEntity == NULL)
		return original(ecx, edx, input_sample_frametime, pCmd);

	const auto pWeapon = pBaseEntity->GetActiveWeapon();
	if (pWeapon == NULL)
		return original(ecx, edx, input_sample_frametime, pCmd);
	gLocalPlayer = pBaseEntity;
	gLocalWeapon = pWeapon;
	INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(i::engine->GetNetChannelInfo());
	if (pNetChannel)
	{
		m_Latency = pNetChannel->GetLatency(0);
	}
	gAntiAim.Run(pBaseEntity, pCmd);
	gMisc.Initalize(pBaseEntity, pCmd);
	Crithack.init(pCmd);
	gVisuals.AspectRatio();
	if (Vars::DT::shifting)
	{
		gMisc.DoAntiWarp(pCmd);
	}
	gEnginePrediction.Start(pCmd);
	{
		gAutoBackstab.Run(pCmd);
		gHitscan.Run(pBaseEntity, pCmd);
		gProjectileAim.Run(pBaseEntity, pCmd);
		gAutoDetonate.Run(pBaseEntity, pCmd);
	}
	gEnginePrediction.End(pCmd);
	gFakelag.Run(pBaseEntity, pCmd);
	if (Vars::DT::shifting) {
		gSendPacket = Vars::DT::Shifted == 1;
	}
	qLASTTICK = pCmd->viewangles;

	static int nChoked = 0;


	/**/
	if (i::clientstate && i::clientstate->chokedcommands > 24)
	{
		gSendPacket = true;
	}
	return false;
}

void FOV(CViewSetup* pView)
{
	CBaseEntity* pLocal = gEntityCache.m_pLocal;

	if (pLocal && pView)
	{
		//if (pLocal->GetObserverMode() == OBS_MODE_FIRSTPERSON || (pLocal->IsScoped() && !Vars::Visuals::RemoveZoom.m_Var))
		//	return;

		pView->fov = Vars::Misc::FOV;

		if (pLocal->IsAlive())
			pLocal->setfov(Vars::Misc::FOV);
	}
}
void __fastcall override_view_model::detour(void* ecx, void* edx, CViewSetup* view)
{
	original(ecx, edx, view);
	FOV(view);
	CBaseEntity* pLocal = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<CBaseEntity>();
	if (!pLocal)
		return original(ecx, edx, view);
	if (pLocal && pLocal->IsScoped() && Vars::Removals::RemoveZoom)
	{
		view->fov = 90;
		view->fovViewmodel = 90;
	}
}

bool __fastcall override_view_model_bool::detour(void* ecx, void* edx, CViewSetup* view)
{
	if (const auto& pLocal = gEntityCache.m_pLocal)
	{
		if (pLocal->IsScoped() && Vars::Removals::RemoveZoom)
			return true;
	}
	return original(ecx, edx, view);
}

bool __fastcall do_post_screen_space_effects::detour(void* ecx, void* edx, const CViewSetup* view) {
	
	//gVisuals.ProjectileTrajectory();
	gChams.Initialize();

	return original(ecx, edx, view);
}

void clientmode_hooks::init() {
	MH_CreateHook(Memory::GetVirtual(i::clientmode, 21), &create_move::detour, reinterpret_cast<void**>(&create_move::original));
	MH_CreateHook(Memory::GetVirtual(i::clientmode, 16), &override_view_model::detour, reinterpret_cast<void**>(&override_view_model::original));
	MH_CreateHook(Memory::GetVirtual(i::clientmode, 16), &override_view_model_bool::detour, reinterpret_cast<void**>(&override_view_model::original));
	MH_CreateHook(Memory::GetVirtual(i::clientmode, 39), &do_post_screen_space_effects::detour, reinterpret_cast<void**>(&do_post_screen_space_effects::original));
}