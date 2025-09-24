#pragma once
#include "SDKClasses.h"
#include "Memory.h"
#include "Entities.h"

class CEntList
{
public:
	IClientEntity* GetClientEntity(int entnum)
	{
		typedef IClientEntity* (__thiscall* OriginalFn)(PVOID, int);
		return Memory::CallVirtual<OriginalFn>(this, 3)(this, entnum);
	}
	IClientEntity* GetClientEntityFromHandle(int hEnt)
	{
		typedef IClientEntity* (__thiscall* OriginalFn)(PVOID, int);
		return Memory::CallVirtual<OriginalFn>(this, 4)(this, hEnt);
	}
	int GetHighestEntityIndex(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 6)(this);
	}
};