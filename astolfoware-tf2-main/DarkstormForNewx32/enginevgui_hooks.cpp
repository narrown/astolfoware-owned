#include "enginevgui_hooks.h"
#include "DrawManager.h"
#include "ESP.h"
#include "Menu.h"
#include "Visuals.h"
#include "Vars.h"
#include "MinHook/MinHook.h"
#include "SpectatorList.h"
using namespace enginevgui_hooks;


enum EPaintMode
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2)
};
void UpdateW2S()
{
	CViewSetup ViewSetup;

	if (i::client->GetPlayerView(ViewSetup)) {
		VMatrix WorldToView, ViewToProjection, WorldToPixels;
		i::renderview->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &Vars::Misc::m_WorldToProjection, &WorldToPixels);
	}
}
const char* returnnigger() {
	if (i::TFGCClientSystem->BHaveChatSuspensionInCurrentMatch()) {
		return "True";
	}
	else {
		return "False";
	}
	
	return "NIGGER";
}
void DebugInfo() {
	if (!i::engine->IsInGame())
		return;
	std::string nigger = "BHaveChatSuspensionInCurrentMatch: " + std::string(returnnigger());
	gDraw.DrawString(5, 35, COLORCODE(0, 255, 0, 255), "Debug Info");
	gDraw.DrawString(5, 45, COLORCODE(255,255,255,255), nigger.c_str());
	/*
	drawmanager::string(5, 35, COLORCODE(0, 255, 0, 255), "Debug Info");
	drawmanager::string(5, 45, COLORCODE(255, 255, 255, 255), net_time.c_str());
	drawmanager::string(5, 55, COLORCODE(255, 255, 255, 255), host_frametime_stddeviation.c_str());
	drawmanager::string(5, 65, COLORCODE(255, 255, 255, 255), host_frametime.c_str());
	drawmanager::string(5, 75, COLORCODE(255, 255, 255, 255), process_ticks.c_str());
	*/

}
void __fastcall paint::detour(void* ecx, void* edx, int mode)
{
	original(ecx, edx, mode);

	if (!gScreenSize.Width || !gScreenSize.Height)
		gDraw.UpdateScreenSize();

	if (mode & PAINT_UIPANELS)
	{
		//Update W2S
		UpdateW2S();

		i::surface->StartDrawing();
		{


			//gDraw.DrawString(5, 5, COLORCODE(255,255,255,255), "shitpaste");
			//DebugInfo();
			gSpectatorList.Run();
			Menu::Get().PaintTraverse();
			gVisuals.Run();
			gESP.Run();
		}
		i::surface->FinishDrawing();
	}
}

void enginevgui_hooks::init() {
	MH_CreateHook(Memory::GetVirtual(i::enginevgui, 14), &paint::detour, reinterpret_cast<void**>(&paint::original));
}