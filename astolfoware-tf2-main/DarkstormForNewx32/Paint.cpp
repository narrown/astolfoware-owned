#include "Paint.h"
#include "Menu.h"
#include "Hash.h"
#include "Vars.h"
int m_nHudZoom = 0;
bool RemoveScope(int nPanel)
{
	if (!m_nHudZoom && Hash::IsHudScope(gInts.Panels->GetName(nPanel)))
		m_nHudZoom = nPanel;

	return (Vars::Removals::RemoveScope && nPanel == m_nHudZoom);
}

void __fastcall Hooked_PaintTraverse(void* pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	try
	{
		if (RemoveScope(vguiPanel))
		{
			return;
		}
		oPaintTraverse(pPanels, edx, vguiPanel, forceRepaint, allowForce);
		static unsigned int vguiFocusOverlayPanel;
		if (vguiFocusOverlayPanel == NULL)
		{
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if (szName[0] == 'F' && szName[5] == 'O' && szName[12] == 'P')
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}

		if (vguiFocusOverlayPanel == vguiPanel)
		{
			gInts.Panels->SetMouseInputEnabled(vguiPanel, Menu::Get().IsVisible());
		}
	}
	catch (...)
	{

	}
}

void Intro(void)
{
	/* This does nothing since i dont need to do anything here. */
}