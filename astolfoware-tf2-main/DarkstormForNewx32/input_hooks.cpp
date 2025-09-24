#include "input_hooks.h"

using namespace input_hooks;

CUserCmd* __fastcall get_user_cmd::detour(void* ecx, void* edx, int sequence_number)
{
	return &i::input->GetCommands()[sequence_number % 90];
}

void input_hooks::init() {
	MH_CreateHook(Memory::GetVirtual(i::input, 8), &get_user_cmd::detour, reinterpret_cast<void**>(&get_user_cmd::original));
}