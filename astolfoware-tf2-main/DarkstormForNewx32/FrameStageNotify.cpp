#include "FrameStageNotify.h"
#include "Misc.h"
#include "CreateMove.h"
#include "CMoveSim.h"
#include "EntityCache.h"
void __fastcall Hooked_FSN(void* ecx, void* edx, client_frame_stage_t stage)
{
	switch (stage)
	{
	case client_frame_stage_t::FRAME_RENDER_START:
	{
		if (gLocalPlayer || gInts.Engine->GetLocalPlayer())
			gMisc.DoThirdPerson();
		/*
		G::PunchAngles = Vec3();

		if (const auto& pLocal = g_EntityCache.GetLocal())
		{
			if (Vars::Visuals::RemovePunch.Value)
			{
				G::PunchAngles = pLocal->GetPunchAngles();
				//Store punch angles to be compesnsated for in aim
				pLocal->ClearPunchAngle(); //Clear punch angles for visual no-recoil
			}


		}
		*/
		break;
	}
	default: break;
	}

	oFSN(ecx, edx, stage);

	switch (stage)
	{
	case client_frame_stage_t::FRAME_NET_UPDATE_START: {
		gEntityCache.Clear(); /* dont we already clear in LevelShutdown?*/
		break;
	}
	case client_frame_stage_t::FRAME_NET_UPDATE_END:
	{
		if (!gLocalPlayer || !gLocalPlayer->GetTeamNum())
			return;
		gEntityCache.Fill();
		gMovementSim.FillVelocities();
		break;
	}
	}
}