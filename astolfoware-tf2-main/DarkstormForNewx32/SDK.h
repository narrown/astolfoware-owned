#pragma once
#include "Vector.h"
#include "Vector2.h"
#include "dt_common2.h"
#include "dt_recv2.h"
#include "Memory.h"
#include <windows.h>
#include <math.h>
#include <xstring>
#include <string>
#include <cstdio>
#include "CNetVars.h"
#include "bitbuf.h"
#include "Enums.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "SignatureManager.h"
#include "SDKClasses.h"

/* interfaces */
#include "chlclient.h"
#include "globals.h"
#include "engine.h"
#include "entlist.h"
#include "isurface.h"
#include "client_mode.h"
#include "prediction.h"
#include "panels.h"
#include "engine_vgui.h"
#include "modelinfo.h"
#include "clientstate.h"
#include "cvars.h"
#include "input.h"
#include "debugoverlay.h"
#include "physics_shit.h"
#include "render_view.h"
#include "CTFGCClientSystem.h"
#include "model_render.h"
#include "MaterialSystem.h"
#include "demo_interface.h"
/* imgui */
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui.h"
/* steam */
#include "Steam/ISteamClient017.h"
#include "Steam/ISteamFriends002.h"
#include "Steam/ISteamUser017.h"
#include "Steam/ISteamFriends015.h"
#include "Steam/ISteamUtils007.h"

#pragma warning(disable : 4002) // SHUSH NIGGER LOL
#pragma warning(disable : 4091)





class ISteamNetworkingUtils /* i do not want to include fedoraware's shitty steam SDK. and i dont need it anyway so we're just doing this. */
{
public:

};

namespace i {
	inline CEntList* entlist = nullptr;
	inline EngineClient* engine = nullptr;
	inline IPanel* panels = nullptr;
	inline ISurface* surface = nullptr;
	inline ClientModeShared* clientmode = nullptr;
	inline CHLClient* client = nullptr;
	inline CGlobals* globals = nullptr;
	inline CMoveHelper* moveHelper = nullptr;
	inline C_Prediction* prediction = nullptr;
	inline C_GameMovement* gamemovement = nullptr;
	inline int* randomseed = nullptr;
	inline IEngineVGUI* enginevgui = nullptr;
	inline IVModelInfo* modelinfo = nullptr;
	inline IEngineTrace* enginetrace = nullptr;
	inline CClientState* clientstate = nullptr;
	inline ICvar* cvars = nullptr;
	inline CInput* input = nullptr;
	inline CDebugOverlay* debugoverlay = nullptr;
	inline IPhysicsCollision* physicscollision = nullptr;
	inline IPhysicsSurfaceProps* physicssurfaceprops = nullptr;
	inline i_physics* physics = nullptr;
	inline IVRenderView* renderview = nullptr;
	inline void** predictionplayer = nullptr;
	inline IDemoPlayer* demoplayer = nullptr;
	inline IDemoRecorder* demorecorder = nullptr;
	inline IMaterialSystem* materialsystem = nullptr;
	inline IVModelRender* modelrender = nullptr;
	inline ISteamClient017* steamclient = nullptr;
	inline ISteamFriends002* steamfriends002 = nullptr;
	inline ISteamUser017* steamuser = nullptr;
	inline ISteamFriends015* friends015 = nullptr;
	inline ISteamUtils007* utils007 = nullptr;
	inline ISteamNetworkingUtils* networkingutils = nullptr;
	inline CTFGCClientSystem* TFGCClientSystem = nullptr;

	inline WNDPROC WndProc = nullptr;
	inline HWND hWindow = nullptr;
	inline IDirect3DDevice9* pDevice = NULL;
}

#define TICK_INTERVAL		( i::globals->interval_per_tick )
#define TIME_TO_TICKS( dt )	( static_cast<int>( 0.5f + static_cast<float>(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )	( TICK_INTERVAL * ( t ) )
#define me i::engine->GetLocalPlayer()
#define GetBaseEntity i::entlist->GetClientEntity
#define MASK_AIMBOT 0x200400B

#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define BLU_TEAM 3
#define RED_TEAM 2