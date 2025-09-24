#include "client.h"
#include "Misc.h"
#include "CreateMove.h"
#include "CMoveSim.h"
#include "EntityCache.h"
#include "MinHook/MinHook.h"
#include "Vars.h"
#include "SignatureManager.h"
#include "Utils.h"

using namespace chlclient_hooks;

void __fastcall frame_stage_notify::detour(void* ecx, void* edx, client_frame_stage_t stage)
{
	switch (stage)
	{
	case client_frame_stage_t::FRAME_RENDER_START:
	{
		if (gLocalPlayer || i::engine->GetLocalPlayer())
			gMisc.DoThirdPerson();

		break;
	}
	default: break;
	}

	original(ecx, edx, stage);

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
void __fastcall level_shutdown::detour(void* ecx, void* edx)
{
	original(ecx, edx);

	gEntityCache.Clear();
}

static int anti_auto_balance = 0;
static const char* previous_server = { };

bool __fastcall dispatch_user_message::detour(void* ecx, void* edx, int msg_type, bf_read& msg_data)
{
	auto data = reinterpret_cast<const char*>(msg_data.m_pData);
	msg_data.SetAssertOnOverflow(false);

	if (data) {
		switch (msg_type)
		{
		case TextMsg: {
			if (msg_data.GetNumBitsLeft() > 35) {
				if (strstr(reinterpret_cast<char*>(msg_data.m_pData), "#TF_Autobalance_TeamChangePending")) {
					INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(i::engine->GetNetChannelInfo());

					if (pNetChannel && pNetChannel->GetAddress() != previous_server) {
						previous_server = pNetChannel->GetAddress();
						anti_auto_balance = 0;
					}

					if (anti_auto_balance < 2) {
						i::engine->ClientCmd_Unrestricted("retry");
					}

					anti_auto_balance++;
				}
			}

			break;
		}
		case VGUIMenu: {
			if (Vars::Misc::SkipWelcome) {
				if (strcmp(reinterpret_cast<char*>(msg_data.m_pData), "info") == 0) {
					//ctx->m_engine_client->client_cmd_unrestricted("closedwelcomemenu");
					i::engine->ClientCmd_Unrestricted("closedwelcomemenu");
					return true;
				}
			}

			break;
		}
		}
	}

	msg_data.Seek(0);
	return original(ecx, edx, msg_type, msg_data);
}

#include <stdio.h>
#include <intrin.h>
#include <map>
struct VelFixRecord {
	Vector m_vecOrigin;
	int m_nFlags;
	float m_flSimulationTime;
};
std::map <C_BasePlayer*, VelFixRecord> velFixRecord;
void __fastcall set_abs_velocity::detour(void* ecx, void* edx, const Vector& vecAbsVelocity) {
	static DWORD dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall = gSignature.GetClientSignature("E8 ? ? ? ? 53 8B CF E8 ? ? ? ? 8D 47 F8 39 05") + 0x5;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall)
	{
		if (auto pPlayer = static_cast<C_BasePlayer*>(ecx))
		{
			if (velFixRecord.find(pPlayer) != velFixRecord.end())
			{
				const auto& Record = velFixRecord[pPlayer];

				float flSimTimeDelta = pPlayer->GetSimulationTime() - Record.m_flSimulationTime;

				if (flSimTimeDelta > 0.0f)
				{
					Vector vOldOrigin = Record.m_vecOrigin;

					int nCurFlags = pPlayer->m_flags();
					int nOldFlags = Record.m_nFlags;

					if (!(nCurFlags & FL_ONGROUND) && !(nOldFlags & FL_ONGROUND))
					{
						bool bCorrected = false;

						if ((nCurFlags & FL_DUCKING) && !(nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z += 20.0f;
							bCorrected = true;
						}

						if (!(nCurFlags & FL_DUCKING) && (nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z -= 20.0f;
							bCorrected = true;
						}

						if (bCorrected)
						{
							Vector vNewVelocity = vecAbsVelocity;
							vNewVelocity.z = (pPlayer->m_vecOrigin().z - vOldOrigin.z) / flSimTimeDelta;
							original(ecx, edx, vNewVelocity);
							return;
						}
					}
				}
			}
		}
	}

	original(ecx, edx, vecAbsVelocity);
}

bool __fastcall in_cond::detour(void* ecx, void* edx, int cond)
{
	if (cond == TF_COND_ZOOMED)
	{
		static const auto dwPlayerShouldDraw = gSignature.GetClientSignature("E8 ? ? ? ? 84 C0 74 05 5E") + 0x5;
		static const auto dwWearableShouldDraw = gSignature.GetClientSignature("E8 ? ? ? ? 84 C0 0F 85 A9 01 00 00 6A 03") + 0x5;

		const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

		if (dwRetAddr == dwPlayerShouldDraw || dwRetAddr == dwWearableShouldDraw)
			return false;
	}
	return original(ecx, edx, cond);
}

void __fastcall update_player_avatar::detour(void* ecx, void* edx, int index, KeyValues* kv) {
	if (!Vars::Misc::StreamerMode) {
		return original(ecx, edx, index, kv);
	}
	if (index == i::engine->GetLocalPlayer()) { // hide local player
		return;
	}
	if (gEntityCache.Friends[index]) { // hide pookies.
		return; 
	}
}
const char* __fastcall get_player_name::detour(void* ecx, void* edx, int index) {
	if (!Vars::Misc::StreamerMode) {
		return original(ecx, edx, index);
	}
	if (index == i::engine->GetLocalPlayer()) {
		return "Local";
	}
	if (gEntityCache.Friends[index]) {
		return "Steam Friend";
	}
	return original(ecx, edx, index);
	/* toDO: Marked Names/RijiN known cheaters name thing */
}

/*
* toDO: this hook, makes it so you can see the classes on the other team.
HOOK_POINTER( IsPlayerDominated, "client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B F8 85 FF 75 08 5F 32 C0 5E 5D C2 04 00", bool, __fastcall, void* ECX, void* EDX, int index )
{
	bool result = OriginalFunction( ECX, EDX, index );

	if ( !result )
	{
		if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == G->IsPlayerDominatedReturnAddressDesired )
			*reinterpret_cast< uintptr_t* >( _AddressOfReturnAddress( ) ) = G->ScoreboardReturnAddressJump;
	}

	return result;
}
*/

void chlclient_hooks::init() {
	auto SetAbsVelocityTarget = reinterpret_cast<set_abs_velocity::oSetAbsVelocity_fn>(sigs::setabsvelocity);
	auto incond = reinterpret_cast<in_cond::InCond_FN>(sigs::in_cond);
	auto update = reinterpret_cast<update_player_avatar::fn>(sigs::update_player_avatar);
	auto player_name = reinterpret_cast<get_player_name::fn>(sigs::get_player_name);

	MH_CreateHook(Memory::GetVirtual(i::client, 36), &dispatch_user_message::detour, reinterpret_cast<void**>(&dispatch_user_message::original));
	MH_CreateHook(Memory::GetVirtual(i::client, 7), &level_shutdown::detour, reinterpret_cast<void**>(&level_shutdown::original));
	MH_CreateHook(Memory::GetVirtual(i::client, 35), &frame_stage_notify::detour, reinterpret_cast<void**>(&frame_stage_notify::original));
	MH_CreateHook(SetAbsVelocityTarget, &set_abs_velocity::detour, reinterpret_cast<LPVOID*>(&set_abs_velocity::original));
	MH_CreateHook(incond, &in_cond::detour, reinterpret_cast<void**>(&in_cond::original));
	MH_CreateHook(update, &update_player_avatar::detour, reinterpret_cast<void**>(&update_player_avatar::original));
	MH_CreateHook(player_name, &get_player_name::detour, reinterpret_cast<void**>(&get_player_name::original));
}