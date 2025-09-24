#include "Hooked_EngineVGUIPaint.h"
#include "DrawManager.h"
#include "ESP.h"
#include "Menu.h"
#include "Visuals.h"
#include "Vars.h"
enum EPaintMode
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2)
};
void UpdateW2S()
{
	CViewSetup ViewSetup;

	if (gInts.Client->GetPlayerView(ViewSetup)) {
		VMatrix WorldToView, ViewToProjection, WorldToPixels;
		gInts.RenderView->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &Vars::Misc::m_WorldToProjection, &WorldToPixels);
	}
}
void __fastcall Hooked_EngineVGUIPaint(void* ecx, void* edx, int mode)
{
	oPaint(ecx, edx, mode);
	
	if (!gScreenSize.Width || !gScreenSize.Height)
		gDraw.UpdateScreenSize();
	
	if (mode & PAINT_UIPANELS)
	{
		//Update W2S
		UpdateW2S();

		gInts.Surface->StartDrawing();
		{
			

			//gDraw.DrawString(5, 5, COLORCODE(255,255,255,255), "shitpaste");
			Menu::Get().PaintTraverse();
			gVisuals.Run();
			gESP.Run();
		}
		gInts.Surface->FinishDrawing();
	}
}