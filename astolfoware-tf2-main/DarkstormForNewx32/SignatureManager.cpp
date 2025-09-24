#include "SignatureManager.h"


void sigs::init()
{
	calcisattackcriticalhelper = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 56 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 ? 89 75 ? 85 F6");
	calcisattackcriticalhelpermelee = Memory::find_signature<DWORD>("client.dll", "55 8B EC A1 ? ? ? ? 83 EC 08 83 78 30 00 57");
	estimateabsvelocity = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 56 8B F1 E8 ? ? ? ? 3B F0 75 ? 8B CE");
	globalvars = Memory::find_signature<DWORD, 1u>("engine.dll", "68 ? ? ? ? 50 50 FF 12");
	movehelper = Memory::find_signature<DWORD, 2u>("client.dll", "8B 0D ? ? ? ? 8B 01 FF 50 28 56 8B C8");
	demorecorder = Memory::find_signature<DWORD, 2u>("engine.dll", "8B 0D ? ? ? ? 0F B6 F0 F7 DE 8B 01 1B F6 83 E6 ? 8B 40 ? FF D0 0F B6 D8"); 
	demoplayer = Memory::find_signature<DWORD, 2u>("engine.dll", "8B 0D ? ? ? ? 85 C9 74 ? 8B 01 8B 40 ? FF D0 84 C0 74 ? 8B 0D ? ? ? ? 8B 01 FF 50 ? 83 F8 ? 7D ? 53");
	randomseed = Memory::find_signature<DWORD, 2u>("client.dll", "C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34");
	clientstate = Memory::find_signature<DWORD, 1u>("engine.dll", "68 ? ? ? ? E8 ? ? ? ? 83 C4 08 5F 5E 5B 5D C3");
	prediction_player = Memory::find_signature<DWORD, 2u>("client.dll", "89 3D ? ? ? ? F3");
	setabsvelocity = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 56 57 8B 7D ? 8B F1 F3 0F 10 07");
	in_cond = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC 08 56 57 8B 7D 08 8B F1 83 FF 20");
	cl_move = Memory::find_signature<DWORD>("engine.dll", "55 8B EC B8 2C 10 00 00 E8 ? ? ? ? 83");
	calculatemaxspeed = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC 18 83 3D ? ? ? ? ? 56");
	is_paused = Memory::find_signature<DWORD>("engine.dll", "80 B9 ? ? ? ? ? 75 42");
	net_time = Memory::find_signature<DWORD, 2u>("engine.dll", "DD 05 ? ? ? ? DD 05 ? ? ? ? DF F1");
	host_frametime_unbounded = Memory::find_signature<DWORD, 2u>("engine.dll", "D9 05 ? ? ? ? DE C2");
	host_frametime_stddeviation = Memory::find_signature<DWORD, 4u>("engine.dll", "F3 0F 10 05 ? ? ? ? 6A ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C6 45 ? ? 89 45 ? F3 0F 11 45 ? 8B 01 6A ? 52 FF 90 ? ? ? ? 8B 4D ? C7 45 ? ? ? ? ? 85 C9 74 ? 8B 01 6A ? FF 10 8B 0D");
	sv_pure = Memory::find_signature<DWORD>("engine.dll", "55 8B EC 83 EC ? 83 3D ? ? ? ? ? 0F 8E");
	create_material_sig = Memory::find_signature<DWORD>("MaterialSystem.dll", "55 8B EC 56 57 FF 75 ? 8B F9 68");
	key_values_init = Memory::find_signature<DWORD>("engine.dll", "FF 15 ? ? ? ? 83 C4 ? 89 06 8B C6");
	key_values_loadfrombuffer = Memory::find_signature<DWORD>("engine.dll", "55 8B EC 83 EC ? 53 8B 5D ? 89 4D ? 85 DB");
	send_datagram = Memory::find_signature<DWORD>("engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9");
	start_drawing = Memory::find_signature<DWORD>("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
	finish_drawing = Memory::find_signature<DWORD>("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");
	sphere_thing = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC 14 D9 45 0C");
	GTFGCClientSystem = Memory::find_signature<DWORD, 1u>("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 6A"); // 1 offset in? cant you just get it from mov ecx?
	update_assigned_lobby = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 53 8B D9 56 57 89 5D ? 8B 8B ? ? ? ? 85 C9 0F 84");
	update_player_avatar = Memory::find_signature<DWORD>("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B 5D ? 56 8B F1 85 DB 0F 84 ? ? ? ? 8B 06");
	get_player_name = Memory::find_signature<DWORD>("client.dll", "55 8B EC 56 8B 75 ? 57 8B F9 8D 46 ? 83 F8");
}
