#pragma once
#include "SDK.h"
enum class client_frame_stage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};
typedef void(__fastcall* FSN_t)(void*, void*, client_frame_stage_t);
inline FSN_t oFSN;
void __fastcall Hooked_FSN(void* ecx, void* edx, client_frame_stage_t stage);