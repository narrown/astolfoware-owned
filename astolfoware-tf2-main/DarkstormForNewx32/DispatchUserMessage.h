#pragma once
#include "SDK.h"


typedef bool(__fastcall* oDispatchUserMessage_FN)(void*, void*, int msg_type, bf_read& msg_data);
inline oDispatchUserMessage_FN oDispatchUserMessage;
bool __fastcall Hooked_DispatchUserMessage(void* ecx, void* edx, int msg_type, bf_read& msg_data);
