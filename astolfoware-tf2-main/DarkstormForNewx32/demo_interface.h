#pragma once
#include "Memory.h"

class IDemoRecorder
{
public:
	bool IsRecording()
	{
		return Memory::CallVirtual<bool(__thiscall*)(void*)>(this, 4)(this);
	}

	void RecordUserInput(int cmdnumber)
	{
		return Memory::CallVirtual<void(__thiscall*)(void*, int)>(this, 9)(this, cmdnumber);
	}
};

class IDemoPlayer
{
public:
	bool IsPlayingBack()
	{
		return Memory::CallVirtual<bool(__thiscall*)(void*)>(this, 6)(this);
	}
};