#include "SetAbsVelocity.h"
#include <stdio.h>
#include <intrin.h>
#include <map>
struct VelFixRecord {
	Vector m_vecOrigin;
	int m_nFlags;
	float m_flSimulationTime;
};
std::map < C_BasePlayer*, VelFixRecord> velFixRecord;
void __fastcall Hooked_SetAbsVelocity(void* ecx, void* edx, const Vector& vecAbsVelocity) {
	static DWORD dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall = gSignature.GetClientSignature("E8 ? ? ? ? 53 8B CF E8 ? ? ? ? 8D 47 F8 39 05") + 0x5;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall)
	{
		if (auto pPlayer = static_cast<C_BasePlayer*>(ecx))
		{
			if (velFixRecord.find(pPlayer) != velFixRecord.end())
			{
				const auto& Record = velFixRecord[pPlayer];

				float flSimTimeDelta = pPlayer->GetSimulationTime() - Record.m_flSimulationTime;

				if (flSimTimeDelta > 0.0f)
				{
					Vector vOldOrigin = Record.m_vecOrigin;

					int nCurFlags = pPlayer->m_flags();
					int nOldFlags = Record.m_nFlags;

					if (!(nCurFlags & FL_ONGROUND) && !(nOldFlags & FL_ONGROUND))
					{
						bool bCorrected = false;

						if ((nCurFlags & FL_DUCKING) && !(nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z += 20.0f;
							bCorrected = true;
						}

						if (!(nCurFlags & FL_DUCKING) && (nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z -= 20.0f;
							bCorrected = true;
						}

						if (bCorrected)
						{
							Vector vNewVelocity = vecAbsVelocity;
							vNewVelocity.z = (pPlayer->m_vecOrigin().z - vOldOrigin.z) / flSimTimeDelta;
							oSetAbsVelocity(ecx, edx, vNewVelocity);
							return;
						}
					}
				}
			}
		}
	}

	oSetAbsVelocity(ecx, edx, vecAbsVelocity);
}

void __fastcall Hooked_EstimateAbsVelocity(void* ecx, void* edx, Vector& vel) 
{
	if (auto pEntity = reinterpret_cast<CBaseEntity*>(ecx))
	{
		if (pEntity->GetClientClass()->iClassID == CTFPlayer)
		{
			if (auto pPlayer = pEntity->as<C_BasePlayer>()) {
				vel = pPlayer->m_velocity();
				return;
			}
		}
	}
	return oEstimateAbsVelocity(ecx, edx, vel);
}