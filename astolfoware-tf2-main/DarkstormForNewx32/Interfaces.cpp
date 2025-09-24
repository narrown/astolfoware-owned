#include "Interface.h"
#include "DrawManager.h"
#include "CConsole.h"
#include "SignatureManager.h"

void SteamInit()
{
	Memory::GetInterface<ISteamClient017*>(i::steamclient, "SteamClient.dll", "SteamClient017");
	/* steam */
	HSteamPipe hNewPipe = i::steamclient->CreateSteamPipe();
	HSteamUser hNewUser = i::steamclient->ConnectToGlobalUser(hNewPipe);

	i::steamfriends002 = reinterpret_cast<ISteamFriends002*>(i::steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_002));
	i::steamuser = reinterpret_cast<ISteamUser017*>(i::steamclient->GetISteamUser(hNewUser, hNewPipe, STEAMUSER_INTERFACE_VERSION_017));
	i::friends015 = reinterpret_cast<ISteamFriends015*>(i::steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_015));
	i::utils007 = reinterpret_cast<ISteamUtils007*>(i::steamclient->GetISteamUtils(hNewUser, STEAMUTILS_INTERFACE_VERSION_007));
	//static auto fn2 = reinterpret_cast<ISteamNetworkingUtils * (__cdecl*)()>(GetProcAddress(GetModuleHandleA("steamnetworkingsockets.dll"), "SteamNetworkingUtils_LibV4"));
	//gInts.NetworkingUtils = fn2();
}

void interfaces::init()
{
	Memory::GetInterface<EngineClient*>(i::engine, "engine.dll", "VEngineClient013");
	Memory::GetInterface<CHLClient*>(i::client, "client.dll", "VClient017");
	Memory::GetInterface<IPanel*>(i::panels, "vgui2.dll", "VGUI_Panel009");
	Memory::GetInterface<ISurface*>(i::surface, "vguimatsurface.dll", "VGUI_Surface030");
	Memory::GetInterface<CEntList*>(i::entlist, "client.dll", "VClientEntityList003");
	Memory::GetInterface<C_Prediction*>(i::prediction, "client.dll", "VClientPrediction001");
	Memory::GetInterface<C_GameMovement*>(i::gamemovement, "client.dll", "GameMovement001");
	Memory::GetInterface<IEngineVGUI*>(i::enginevgui, "engine.dll", "VEngineVGui002");
	Memory::GetInterface<IEngineTrace*>(i::enginetrace, "engine.dll", "EngineTraceClient003");
	Memory::GetInterface<IVModelInfo*>(i::modelinfo, "engine.dll", "VModelInfoClient006");
	Memory::GetInterface<ICvar*>(i::cvars, "vstdlib.dll", "VEngineCvar004");
	Memory::GetInterface<CDebugOverlay*>(i::debugoverlay, "engine.dll", "VDebugOverlay003");
	Memory::GetInterface<i_physics*>(i::physics, "vphysics.dll", "VPhysics031");
	Memory::GetInterface<IPhysicsCollision*>(i::physicscollision, "vphysics.dll", "VPhysicsCollision007");
	Memory::GetInterface< IVRenderView*>(i::renderview, "engine.dll", "VEngineRenderView014");
	Memory::GetInterface<IMaterialSystem*>(i::materialsystem, "materialsystem.dll", "VMaterialSystem082");
	Memory::GetInterface<IVModelRender*>(i::modelrender, "engine.dll", "VEngineModel016");

	//LLM technologies : Filter = **reinterpret_cast<IPredictionSystem***>(g_Pattern.Find(CLIENT, L'A1 ? ? ? ? 0F 94 C1') + 0x1);

	SteamInit();
	/* clientmode  (nothing changed, still 10 + 5) */
	DWORD dwHudProcessInput = (DWORD)((*(void***)i::client)[10]);
	DWORD dwClientModeAddress = dwHudProcessInput + 0x5;
	DWORD nigger = (DWORD)((*(void***)i::client)[14]);
	DWORD full = nigger + 2;

	i::clientmode = **(ClientModeShared***)(dwClientModeAddress);
	i::input = **(CInput***)(full); 
	i::globals = *reinterpret_cast<CGlobals**>(sigs::globalvars);
	i::randomseed = *reinterpret_cast<int32_t**>(sigs::randomseed);
	i::clientstate = *reinterpret_cast<CClientState**>(sigs::clientstate);
	i::predictionplayer = *reinterpret_cast<void***>(sigs::prediction_player);
	i::moveHelper =   **reinterpret_cast<CMoveHelper***>(sigs::movehelper);
	i::demoplayer =   **reinterpret_cast<IDemoPlayer***>(sigs::demoplayer);
	i::demorecorder = **reinterpret_cast<IDemoRecorder***>(sigs::demorecorder);
	//  void *dword_10CD5590?
	i::TFGCClientSystem = **reinterpret_cast<CTFGCClientSystem***>(sigs::GTFGCClientSystem);
	gNetVars.Initialize();
	gDraw.Initialize();
}