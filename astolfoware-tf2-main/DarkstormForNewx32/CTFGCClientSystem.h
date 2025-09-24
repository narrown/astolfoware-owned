#pragma once
#include "Memory.h"

/* B9 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 6A */
/* mov ecx, offset dword_10CD5590 */
class CTFGCClientSystem {
public:
	/* i dont really wanna do offsets but i have no other choice. */
	bool BHaveChatSuspensionInCurrentMatch() {
		// bSuspensionInMatch = (int)GTFGCClientSystem__() && *((_BYTE *)GTFGCClientSystem__() + 0x5A0);
		return *reinterpret_cast<bool*>(uintptr_t(this) + 0x5A0); // decimal: 1440
	}
	void SetChatSuspensionInCurrentMatch(bool state) {
		*reinterpret_cast<bool*>(uintptr_t(this) + 0x5A0) = state;
	}
};