#pragma once
#include "SDK.h"
#include "CreateMove.h"
#include "RunCommand.h"
#include "Menu.h"
#include "ImGui/imgui.h"
#include "SignatureManager.h"

/*
class CHooks
{
public:
	void Initalize();
	void Uninitalize(LPVOID lpArguments);
};

inline CHooks* Hooks;
*/

namespace hooks {
	void init();
	void uninit(LPVOID lpArguments);
}