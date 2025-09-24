#include "GetUserCmd.h"

CUserCmd* __fastcall Hooked_GetUserCmd(void* ecx, void* edx, int sequence_number)
{
	return &gInts.Input->GetCommands()[sequence_number % 90];
}