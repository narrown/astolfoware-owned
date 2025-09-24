#pragma once
#include "Memory.h"

class IPanel
{
public:
	const char* GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
		return Memory::CallVirtual<OriginalFn>(this, 36)(this, vguiPanel);
	}
	void SetMouseInputEnabled(unsigned int panel, bool state)
	{
		Memory::CallVirtual<void(__thiscall*)(void*, int, bool)>(this, 32)(this, panel, state);
	}
};