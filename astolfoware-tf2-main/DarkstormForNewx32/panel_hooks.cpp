#include "panel_hooks.h"
#include "Vars.h"
#include "Hash.h"
#include "Menu.h"
#include "MinHook/MinHook.h"
#include "FNV1A.h"

using namespace panel_hooks;

int m_nHudZoom = 0;
bool RemoveScope(int nPanel)
{
	if (!m_nHudZoom && Hash::IsHudScope(i::panels->GetName(nPanel)))
		m_nHudZoom = nPanel;

	return (Vars::Removals::RemoveScope && nPanel == m_nHudZoom);
}

void __fastcall paint::detour(void* pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	try
	{
		if (RemoveScope(vguiPanel))
		{
			return;
		}

		constexpr auto HudScope = FNV1A::HashConst("HudScope");
		constexpr auto SteamFriendsList = FNV1A::HashConst("SteamFriendsList");
		constexpr auto RankPanel = FNV1A::HashConst("RankPanel");
		constexpr auto avatar = FNV1A::HashConst("avatar");

		const auto PanelName = i::panels->GetName(vguiPanel);

		if (Vars::Misc::StreamerMode)
		{
			if (FNV1A::Hash(PanelName) == SteamFriendsList ||
				FNV1A::Hash(PanelName) == RankPanel ||
				FNV1A::Hash(PanelName) == avatar)
			{
				return;
			}
		}

		original(pPanels, edx, vguiPanel, forceRepaint, allowForce);
		static unsigned int vguiFocusOverlayPanel;
		if (vguiFocusOverlayPanel == NULL)
		{
			const char* szName = i::panels->GetName(vguiPanel);
			if (szName[0] == 'F' && szName[5] == 'O' && szName[12] == 'P')
			{
				vguiFocusOverlayPanel = vguiPanel;
				//Intro();
			}
		}

		if (vguiFocusOverlayPanel == vguiPanel)
		{
			i::panels->SetMouseInputEnabled(vguiPanel, Menu::Get().IsVisible());
		}
	}
	catch (...)
	{

	}
}

void panel_hooks::init() {
	MH_CreateHook(Memory::GetVirtual(i::panels, 41), &paint::detour, reinterpret_cast<void**>(&paint::original));
}