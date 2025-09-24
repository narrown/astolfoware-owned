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
#define DT_WAIT_CALLS 26

bool __fastcall Hooked_CreateMove(void* ecx, void* edx, float input_sample_frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number || !pCmd)
		return oCreateMove(ecx, edx, input_sample_frametime, pCmd);

	CBaseEntity* pBaseEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer())->as<CBaseEntity>();
	if (pBaseEntity == NULL)
		return oCreateMove(ecx, edx, input_sample_frametime, pCmd);

	const auto pWeapon = pBaseEntity->GetActiveWeapon();
	if (pWeapon == NULL)
		return oCreateMove(ecx, edx, input_sample_frametime, pCmd);
	gLocalPlayer = pBaseEntity;
	gLocalWeapon = pWeapon;
	INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(gInts.Engine->GetNetChannelInfo());
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
		
		gHitscan.Run(pBaseEntity, pCmd);
		//projectile(pCmd);
		gProjectileAim.Run(pBaseEntity, pCmd);
		gAutoDetonate.Run(pBaseEntity, pCmd);
	}
	gEnginePrediction.End(pCmd);
	gFakelag.Run(pBaseEntity, pCmd);
	if (Vars::DT::shifting) {
		gSendPacket = Vars::DT::Shifted == 2 + 1;
	}
	qLASTTICK = pCmd->viewangles;
	
	static int nChoked = 0;
	

	/**/
	if (gInts.ClientState && gInts.ClientState->chokedcommands > 22)
	{
		gSendPacket = true;
	}
	return false;
}