#include "OverrideViewmodel.h"
#include "EntityCache.h"
#include "Vars.h"
#include "CreateMove.h"
void FOV(CViewSetup* pView)
{
	CBaseEntity* pLocal = gEntityCache.m_pLocal;

	if (pLocal && pView)
	{
		//if (pLocal->GetObserverMode() == OBS_MODE_FIRSTPERSON || (pLocal->IsScoped() && !Vars::Visuals::RemoveZoom.m_Var))
		//	return;

		pView->fov = Vars::Misc::FOV;

		if (pLocal->IsAlive())
			pLocal->setfov(Vars::Misc::FOV);
	}
}
void __fastcall Hooked_OverrideViewModel(void* ecx, void* edx, CViewSetup* view)
{
	oOverrideViewmodel(ecx, edx, view);
	FOV(view);
	CBaseEntity* pLocal = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer())->as<CBaseEntity>();
	if (!pLocal)
		return oOverrideViewmodel(ecx, edx, view);
	if (pLocal && pLocal->IsScoped() && Vars::Removals::RemoveZoom)
	{
		view->fov = 90;
		view->fovViewmodel = 90;
	}
}

bool __fastcall Hooked_OverrideViewModelBool(void* ecx, void* edx, CViewSetup* view)
{
	if (const auto& pLocal = gEntityCache.m_pLocal)
	{
		if (pLocal->IsScoped() && Vars::Removals::RemoveZoom)
			return true;
	}
	return oOverrideViewmodelBool(ecx, edx, view);
}