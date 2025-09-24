#include "SetChoked.h"
/* "engine.dll", "FF 41 ? FF 41" */
void __fastcall Hooked_SetChoked(CNetChannel* ecx, void* edx)
{
	if (!ecx)
	{
		return oSetChoked(ecx, edx);
	}

	const auto ChokedBackup = ecx->ChokedPackets;
	ecx->ChokedPackets = 0;
	ecx->SendDatagram(NULL);
	--ecx->OutSequenceNr;
	ecx->ChokedPackets = ChokedBackup;

	return oSetChoked(ecx, edx);
}

/*
//hooking this and firing events here (or cl_move) sometimes fucks over player data (like origin)
//because we're calling it too early (before we fully receive server data)
//this pasted supremacy shit doesn't work properly, don't hook, maybe only if we do fakelatency
HOOK_POINTER( ProcessPacket, "engine.dll", "55 8B EC 83 EC ? 53 56 57 8B 7D ? 8B D9 83 7F",
			  void, __fastcall, void* ECX, void* EDX, void* packet, bool header )
{
	auto NetChan = reinterpret_cast< CNetChannel* >( ECX );
	auto Packet = reinterpret_cast< CNetChannel::netpacket_t* >( packet );

	

OriginalFunction(ECX, EDX, packet, header);



TF2->UpdateIncomingSequences();

gEngineClient->FireEvents();
}
*/