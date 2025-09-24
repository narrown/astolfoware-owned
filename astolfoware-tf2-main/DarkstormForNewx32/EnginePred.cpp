#include "EnginePred.h"
#include "MD5.h"
#include "Vars.h"
#include "Misc.h"
int CEnginePrediction::GetTickbase(CUserCmd* pCmd, C_BasePlayer* pLocal)
{
	static int nTick = 0;
	static CUserCmd* pLastCmd = nullptr;

	if (pCmd)
	{
		if (!pLastCmd || pLastCmd->hasbeenpredicted)
			nTick = pLocal->GetTickBase();

		else nTick++;

		pLastCmd = pCmd;
	}

	return nTick;
}
#undef max
void CEnginePrediction::Start(CUserCmd* pCmd)
{
	C_BasePlayer* pLocal = GetBaseEntity(me)->as<C_BasePlayer>();

	if (pLocal && pLocal->IsAlive() && i::moveHelper)
	{
		m_fOldCurrentTime = i::globals->curtime;
		m_fOldFrameTime = i::globals->frametime;
		m_nOldTickCount = i::globals->tickcount;

		const int nOldTickBase = pLocal->GetTickBase();
		const bool bOldIsFirstPrediction = i::prediction->m_bFirstTimePredicted;
		const bool bOldInPrediction = i::prediction->m_bInPrediction;
		const Vector local_velocity = pLocal->m_velocity();

	
		i::moveHelper->SetHost(pLocal);

		pLocal->SetCurrentCmd(pCmd);

		*i::randomseed = MD5_PseudoRandom(pCmd->command_number) & std::numeric_limits<int>::max();


		i::globals->curtime = TICKS_TO_TIME(GetTickbase(pCmd, pLocal));
		i::globals->frametime = (i::prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
		i::globals->tickcount = GetTickbase(pCmd, pLocal);

		i::prediction->m_bFirstTimePredicted = false;
		i::prediction->m_bInPrediction = true;

		
		i::prediction->SetLocalViewAngles(pCmd->viewangles);

		i::prediction->SetupMove(pLocal, pCmd, i::moveHelper, &m_MoveData);
		i::gamemovement->ProcessMovement(pLocal, &m_MoveData); 
		i::prediction->FinishMove(pLocal, pCmd, &m_MoveData);

		i::moveHelper->SetHost(nullptr);

		pLocal->SetTickBase(nOldTickBase);
		pLocal->m_velocity() = local_velocity; 

		i::prediction->m_bInPrediction = bOldInPrediction;
		i::prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	}
}

void CEnginePrediction::End(CUserCmd* pCmd)
{
	C_BasePlayer* pLocal = GetBaseEntity(me)->as<C_BasePlayer>();

	if (pLocal && pLocal->IsAlive() && i::moveHelper)
	{
		pLocal->SetCurrentCmd(nullptr);

		i::globals->curtime = m_fOldCurrentTime;
		i::globals->frametime = m_fOldFrameTime;
		i::globals->tickcount = m_nOldTickCount;


		*i::randomseed = -1;
	}
}