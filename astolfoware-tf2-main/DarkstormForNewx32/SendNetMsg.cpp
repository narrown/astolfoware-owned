#include "SendNetMsg.h"
#include "Vars.h"
bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to)
{
	//MAKE_SIGNATURE(WriteUsercmd, "client.dll", "55 8B EC 83 EC 08 53 8B 5D 0C 56 8B 75 10 57 8B 7D 08 8B 46 04 40 39 43 04 8B 47 0C", 0);
	CUserCmd nullcmd = {};
	CUserCmd* f = nullptr;

	if (from == -1)
		f = &nullcmd;

	else
	{
		f = gInts.Input->get_user_cmd(from);

		if (!f)
			f = &nullcmd;
	}

	CUserCmd* t = gInts.Input->get_user_cmd(to);

	if (!t)
		t = &nullcmd;
	static DWORD WriteUserCmd = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 53 8B 5D ? 56 8B 75 ? 57 8B 7D ? 8B 46 ? 40 39 43 ? 8B 47");
	reinterpret_cast<void(__cdecl*)(bf_write*, CUserCmd*, CUserCmd*)>(WriteUserCmd)(buf, t, f);

	return !buf->m_bOverflow;
}
#define	clc_Move 9
#undef max
bool __fastcall Hooked_SendNetMessage(CNetChannel* pNet, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice) {
	if (msg.GetType() == clc_Move)
	{
		CLC_Move* pMsg = reinterpret_cast<CLC_Move*>(&msg);

		int nLastOutGoingCommand = gInts.ClientState->lastoutgoingcommand;
		int nChokedCommands = gInts.ClientState->chokedcommands;
		int nNextCommandNr = nLastOutGoingCommand + nChokedCommands + 1;

		unsigned char Data[4000] = {};
		pMsg->m_DataOut.start_writing(Data, sizeof(Data));

		pMsg->m_nNewCommands = 1 + nChokedCommands;
		pMsg->m_nNewCommands = std::clamp(pMsg->m_nNewCommands, 0, MAX_NEW_COMMANDS);

		int nExtraCommands = nChokedCommands + 1 - pMsg->m_nNewCommands;
		int nCmdBackup = std::max(2, nExtraCommands);

		pMsg->m_nBackupCommands = std::clamp(nCmdBackup, 0, MAX_BACKUP_COMMANDS);

		int nNumCmds = pMsg->m_nNewCommands + pMsg->m_nBackupCommands;
		int nFrom = -1;

		bool bOK = true;

		for (int nTo = nNextCommandNr - nNumCmds + 1; nTo <= nNextCommandNr; nTo++)
		{
			bOK = bOK && WriteUsercmdDeltaToBuffer(&pMsg->m_DataOut, nFrom, nTo);

			nFrom = nTo;
			gInts.Cvars->ConsolePrintf("[sendnetmsg] nTo: %i", nTo);
		}

		if (bOK)
		{
			
			if (nExtraCommands > 0)
			{
				pNet->ChokedPackets -= nExtraCommands;
			}
			gInts.Cvars->ConsolePrintf("[sendnetmsg] nTo: %i", nExtraCommands);
			oSendNetMsg(pNet, edx, reinterpret_cast<INetMessage&>(*pMsg), bForceReliable, bVoice);
		}
		
		return true;
	}

	return oSendNetMsg(pNet, edx, msg, bForceReliable, bVoice);
}