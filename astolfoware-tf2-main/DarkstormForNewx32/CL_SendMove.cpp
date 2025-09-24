#include "CL_SendMove.h"
#include <math.h>
#include "Math.h"
#define NUM_NEW_COMMAND_BITS		4
#define MAX_NEW_COMMANDS			((1 << NUM_NEW_COMMAND_BITS)-1)
#define NUM_BACKUP_COMMAND_BITS		3
#define MAX_BACKUP_COMMANDS			((1 << NUM_BACKUP_COMMAND_BITS)-1)
#define MAX_PLAYER_NAME_LENGTH		32
#define MAX_CUSTOM_FILES		4		// max 4 files
#define NETMSG_TYPE_BITS	6	// must be 2^NETMSG_TYPE_BITS > SVC_LASTMSG
#undef max
void __fastcall Hooked_CLSendMove(void* ecx, void* edx)
{
	if (!gInts.ClientState || !gInts.ClientState->m_NetChannel) //|| G->DetachSystem->WantsDetach)
		return oSendMove(ecx, edx);


	//NoSpread->CL_SendMoveHandler();

	std::byte data[4000];
	const int Chocked = ((CNetChannel*)gInts.ClientState->m_NetChannel)->ChokedPackets;
	const int NextCommandNumber = gInts.ClientState->lastoutgoingcommand + Chocked + 1;

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
		bOK = bOK && gInts.Client->WriteUsercmdDeltaToBuffer(&Message.m_DataOut, from, to, isnewcmd);
		from = to;
	}

	if (bOK) {
		if (ExtraCommands)
			((CNetChannel*)gInts.ClientState->m_NetChannel)->ChokedPackets -= ExtraCommands;

		Memory::CallVirtual<bool(__thiscall*)(void*, INetMessage*, bool, bool)>(gInts.ClientState->m_NetChannel, 37)(gInts.ClientState->m_NetChannel, &Message, false, false);
	}

	//NoSpread->CL_SendMovePostHandler();
}