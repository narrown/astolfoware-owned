#include "Fakelag.h"
#include "CreateMove.h"
#include "Vars.h"
#include <algorithm> // for std::min

#ifdef min
#undef min
#endif
void CFakeLag::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	// since we have done CL_SendMove rebuild, we can lag for 24-22 ticks.
	// we arent limited to 16 ticks.
	if (pLocal) {
		int m_nMaxLag = 0;
		int m_nLag = 0;
		bool bOnGround = (pLocal->GetFlags() & FL_ONGROUND) != 0;
		m_nMaxLag = 22;
		int nLimit = std::min(Vars::FL::Amount, m_nMaxLag);
		m_nLag = i::clientstate->chokedcommands;
		if (Vars::FL::Fakelag) {
			gSendPacket = false;
			if (m_nLag >= nLimit || (pCmd->buttons & IN_ATTACK) || Vars::DT::Shifted > 2) {
				gSendPacket = true;
			}
		}
	}
}