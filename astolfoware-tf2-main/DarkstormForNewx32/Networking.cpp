#include "Networking.h"
#include "Vars.h"
#include "CreateMove.h"
#include "Utils.h"

#undef max
#undef min

void CNetworking::CL_SendMove()
{
	std::byte data[4000];
	const int Chocked = i::clientstate->m_NetChannel->ChokedPackets;
	const int NextCommandNumber = i::clientstate->lastoutgoingcommand + Chocked + 1;

	CLC_Move Message;
	Message.m_DataOut.start_writing(data, sizeof(data));

	Message.m_nNewCommands = 1 + Chocked;
	Message.m_nNewCommands = std::clamp(Message.m_nNewCommands, 0, MAX_NEW_COMMANDS);
	const int ExtraCommands = (Chocked + 1) - Message.m_nNewCommands;
	const int BackupCommands = std::max(2, ExtraCommands);
	Message.m_nBackupCommands = std::clamp(BackupCommands, 0, MAX_BACKUP_COMMANDS);

	const int numcmds = Message.m_nNewCommands + Message.m_nBackupCommands;

	int from = -1;
	bool bOK = true;
	for (int to = NextCommandNumber - numcmds + 1; to <= NextCommandNumber; to++) {
		const bool isnewcmd = to >= NextCommandNumber - Message.m_nNewCommands + 1;
		bOK = bOK && i::client->WriteUsercmdDeltaToBuffer(&Message.m_DataOut, from, to, isnewcmd);
		from = to;
	}

	if (bOK) {
		if (ExtraCommands)
			i::clientstate->m_NetChannel->ChokedPackets -= ExtraCommands;

		Memory::CallVirtual<bool(__thiscall*)(void*, INetMessage*, bool, bool)>(i::clientstate->m_NetChannel, 37)(i::clientstate->m_NetChannel, &Message, false, false);
	
		Vars::DT::ProcessTicks -= i::clientstate->chokedcommands + 1;
		Vars::DT::ProcessTicks = std::max(0, Vars::DT::ProcessTicks);
	}
}

void CNetworking::CL_Move(float AccumulatedExtraSamples, bool FinalTick)
{
	auto host_limitlocal = i::cvars->FindVar("host_limitlocal");
	auto cl_cmdrate = i::cvars->FindVar("cl_cmdrate");
	//bool should_run = reinterpret_cast<bool(__thiscall*)()>(sigs::s_hostshouldrun)();
	//if (!should_run)
	//	return;

	if (!i::engine->IsConnected())
		return;

	gSendPacket = true;
	double NetTime = **reinterpret_cast<double**>(sigs::net_time);

	if ((!i::clientstate->m_NetChannel->IsLoopback()) &&
		((NetTime < i::clientstate->m_flNextCmdTime) || !i::clientstate->m_NetChannel->CanPacket() || !FinalTick)) {
		gSendPacket = false;
	}


	if (i::clientstate->IsActive()) {
		int nextcommandnr = i::clientstate->lastoutgoingcommand + i::clientstate->chokedcommands + 1;

		Vars::DT::ProcessTicks++;
		Vars::DT::ProcessTicks = std::min(24, Vars::DT::ProcessTicks);

		// Have client .dll create and store usercmd structure
		if (Utils::IsKeyPressed(Vars::DT::Key))
		{
			Vars::DT::Recharging = true;
		}
		if (Vars::DT::Shifted < 21 && Vars::DT::Recharging)
		{
			Vars::DT::Shifted++;
			return;
		}
		else
		{
			Vars::DT::Recharging = false;
		}
		/*
		if (Vars::DT::Recharging && i::clientstate->chokedcommands == 0)
		{
			CUserCmd* const nullcmd = &i::input->GetCommands()[nextcommandnr % 90];
			if (nullcmd)
			{
				memset(nullcmd, 0, sizeof(CUserCmd));

				nullcmd->command_number = nextcommandnr;
				nullcmd->tick_count = INT_MAX;
			}

			goto RECHARGE;
		}
		*/

		i::client->CreateMove(nextcommandnr, i::globals->interval_per_tick - AccumulatedExtraSamples,
			!i::clientstate->IsPaused());



		// Store new usercmd to dem file
		if (i::demorecorder->IsRecording()) {
			// Back up one because we've incremented outgoing_sequence each frame by 1 unit
			i::demorecorder->RecordUserInput(nextcommandnr);
		}

		if (gSendPacket) {
			//cl_sendmove();
			CL_SendMove();
		}
		else {
			// netchanll will increase internal outgoing sequnce number too
			i::clientstate->m_NetChannel->SetChoked();
			// Mark command as held back so we'll send it next time
			i::clientstate->chokedcommands++;
		}
	}
//RECHARGE:
	if (!gSendPacket)
		return;

	// Request non delta compression if high packet loss, show warning message
	bool HasProblem = i::clientstate->m_NetChannel->IsTimingOut() && !i::demoplayer->IsPlayingBack() && i::clientstate->IsActive();
	if (HasProblem) {
		// I'll prob force full update with unrestriced cmd - wraith
		i::clientstate->m_nDeltaTick = -1;
	}
	float unbounded = **reinterpret_cast<float**>(sigs::host_frametime_unbounded);
	float stddeviation = **reinterpret_cast<float**>(sigs::host_frametime_stddeviation);

	if (i::clientstate->IsActive()) {
		NET_Tick mymsg(i::clientstate->m_nDeltaTick, unbounded, stddeviation);
		i::clientstate->m_NetChannel->SendNetMsg(mymsg);
	}

	i::clientstate->lastoutgoingcommand = i::clientstate->m_NetChannel->SendDatagram(NULL);
	i::clientstate->chokedcommands = 0;

	if (i::clientstate->IsActive()) {
		// use full update rate when active
		float commandInterval = 1.0f / cl_cmdrate->GetFloat();
		float maxDelta = std::min(i::globals->interval_per_tick, commandInterval);
		float delta = std::clamp((float)(NetTime - i::clientstate->m_flNextCmdTime), 0.0f, maxDelta);
		i::clientstate->m_flNextCmdTime = NetTime + commandInterval - delta;
		//i::clientstate->m_flNextCmdTime = net_time + i::global_vars->interval_per_tick;
	}
	else {
		// during signon process send only 5 packets/second
		i::clientstate->m_flNextCmdTime = NetTime + (1.0f / 5.0f);
		//i::clientstate->m_flNextCmdTime = NetTime + 0.2f; i dont think this matters
	}

	
}
int CNetworking::GetLatestCommandNumber()
{
	return (i::clientstate->lastoutgoingcommand + i::clientstate->chokedcommands + 1);
}