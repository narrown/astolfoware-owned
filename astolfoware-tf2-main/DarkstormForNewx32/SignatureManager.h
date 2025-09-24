#pragma once
#include "SDK.h"
#include "SigScanner.h"

/*
class CSignatureManager
{
public:
	void Initialize();
public:
	inline static DWORD EstimateAbsVelocity;
	inline static DWORD m_dwCalculateMaxSpeed;
	inline static DWORD CalcIsAttackCriticalHelper;
	inline static DWORD CalcIsAttackCriticalHelperMelee;
	inline static DWORD GlobalVars;
	inline static DWORD dwAttribHookValue;
};
*/
namespace sigs {
	void init();
	/* misc */
	inline DWORD calcisattackcriticalhelper;
	inline DWORD calcisattackcriticalhelpermelee;
	inline DWORD calculatemaxspeed;
	inline DWORD estimateabsvelocity;
	inline DWORD net_time;
	inline DWORD host_frametime_unbounded;
	inline DWORD host_frametime_stddeviation;
	inline DWORD create_material_sig;
	inline DWORD key_values_init;
	inline DWORD key_values_loadfrombuffer;
	inline DWORD send_datagram;
	inline DWORD start_drawing;
	inline DWORD finish_drawing;
	inline DWORD sphere_thing;
	/* interfaces */
	inline DWORD globalvars;
	inline DWORD movehelper;
	inline DWORD demorecorder;
	inline DWORD demoplayer;
	inline DWORD randomseed;
	inline DWORD clientstate;
	inline DWORD prediction_player;
	inline DWORD is_paused;
	inline DWORD GTFGCClientSystem;
	/* hooks */
	inline DWORD setabsvelocity;
	inline DWORD in_cond;
	inline DWORD cl_move;
	inline DWORD sv_pure;
	inline DWORD update_assigned_lobby;
	inline DWORD update_player_avatar;
	inline DWORD get_player_name;
}

