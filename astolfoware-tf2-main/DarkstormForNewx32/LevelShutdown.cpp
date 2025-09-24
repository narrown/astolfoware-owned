#include "LevelShutDown.h"
#include "EntityCache.h"

void __fastcall Hooked_LevelShutdown(void* ecx, void* edx)
{
	oLevelShutdown(ecx, edx);

	gEntityCache.Clear();
}