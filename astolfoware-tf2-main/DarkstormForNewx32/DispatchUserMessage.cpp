#include "DispatchUserMessage.h"
#include "Vars.h"
static int anti_auto_balance = 0;
static const char* previous_server = { };

bool __fastcall Hooked_DispatchUserMessage(void* ecx, void* edx, int msg_type, bf_read& msg_data)
{
	auto data = reinterpret_cast<const char*>(msg_data.m_pData);
	msg_data.SetAssertOnOverflow(false);

	if (data) {
		switch (msg_type) 
		{
		case TextMsg: {
			if (msg_data.GetNumBitsLeft() > 35) {
				//ctx->m_cvar->console_color_printf( { 0, 255, 255, 255 }, "%s\n", data );
				if (strstr(reinterpret_cast<char*>(msg_data.m_pData), "#TF_Autobalance_TeamChangePending")) {
					INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(gInts.Engine->GetNetChannelInfo());
					//if (!pNetChannel)
					//	return;
					if (pNetChannel && pNetChannel->GetAddress() != previous_server) {
						previous_server = pNetChannel->GetAddress();
						anti_auto_balance = 0;
					}

					if (anti_auto_balance < 2) {
						//ctx->m_engine_client->client_cmd_unrestricted("retry");
						gInts.Engine->ClientCmd_Unrestricted("retry");
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
					gInts.Engine->ClientCmd_Unrestricted("closedwelcomemenu");
					return true;
				}
			}

			break;
		}
		}
	}

	msg_data.Seek(0);
	return oDispatchUserMessage(ecx, edx, msg_type, msg_data);
}

/*
void hooks::dispatch_user_message::init() {
	auto target = util::get_virtual< fn >(ctx->m_client, 36u);
	MH_CreateHook(target, &function, reinterpret_cast<LPVOID*>(&original_function));
}
*/