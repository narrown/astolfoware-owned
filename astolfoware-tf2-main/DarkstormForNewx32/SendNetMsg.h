#pragma once
#include "SDK.h"
/*
MAKE_HOOK(
	INetChannel_SendNetMsg, Signatures::INetChannel_SendNetMsg.Get(),
	bool, __fastcall, CNetChannel *pNet, void *edx, INetMessage &msg, bool bForceReliable, bool bVoice)
{
	if (msg.GetType() == clc_Move)
	{
		CLC_Move *pMsg = reinterpret_cast<CLC_Move *>(&msg);

		int nLastOutGoingCommand = I::ClientState->lastoutgoingcommand;
		int nChokedCommands = I::ClientState->chokedcommands;
		int nNextCommandNr = nLastOutGoingCommand + nChokedCommands + 1;

		unsigned char Data[4000] = {};
		pMsg->m_DataOut.StartWriting(Data, sizeof(Data));

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
		}

		if (bOK)
		{
			if (nExtraCommands > 0)
			{
				pNet->m_nChokedPackets -= nExtraCommands;
			}

			CALL_ORIGINAL(pNet, edx, reinterpret_cast<INetMessage &>(*pMsg), bForceReliable, bVoice);
		}

		return true;
	}

	return CALL_ORIGINAL(pNet, edx, msg, bForceReliable, bVoice);
}
*/
typedef bool(__fastcall* oSendNetMsg_fn)(CNetChannel*, void*, INetMessage&, bool, bool);
inline oSendNetMsg_fn oSendNetMsg;
bool __fastcall Hooked_SendNetMessage(CNetChannel* pNet, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);