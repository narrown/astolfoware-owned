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

namespace chlclient_hooks {
	namespace frame_stage_notify {
		typedef void(__fastcall* FSN_t)(void*, void*, client_frame_stage_t);
		inline FSN_t original;
		void __fastcall detour(void* ecx, void* edx, client_frame_stage_t stage);
	}
	namespace level_shutdown {
		typedef void(__fastcall* oLevelShutdown_t)(void*, void*);
		inline oLevelShutdown_t original;
		void __fastcall detour(void* ecx, void* edx);
	}
	namespace dispatch_user_message {
		typedef bool(__fastcall* oDispatchUserMessage_FN)(void*, void*, int msg_type, bf_read& msg_data);
		inline oDispatchUserMessage_FN original;
		bool __fastcall detour(void* ecx, void* edx, int msg_type, bf_read& msg_data);
	}

	namespace set_abs_velocity {
		typedef void(__fastcall* oSetAbsVelocity_fn)(void*, void*, const Vector&);
		inline oSetAbsVelocity_fn original;
		void __fastcall detour(void* ecx, void* edx, const Vector& vecAbsVelocity);
	}

	namespace in_cond {
		typedef bool(__fastcall* InCond_FN)(void*, void*, int);
		inline InCond_FN original;
		bool __fastcall detour(void* ecx, void* edx, int cond);
	}
	namespace update_player_avatar {
		typedef void(__fastcall* fn)(void*, void*, int, KeyValues*);
		inline fn original;
		void __fastcall detour(void* ecx, void* edx, int index, KeyValues* kv);
	}
	namespace get_player_name {
		typedef const char* (__fastcall* fn)(void*, void*, int);
		inline fn original;
		const char* __fastcall detour(void* ecx, void* edx, int index);
	}

	void init();
}