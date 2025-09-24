#include "PrioritySystem.h"
#include "Rage.h"
#include "Utils.h"
#include "Vars.h"
#include "EntityCache.h"
bool CPrioritySystem::ShouldBeIgnored(CBaseEntity* pPlayer)
{
	int Conditions = pPlayer->GetCond();

	if (Conditions & TFCond_Bonked ||
		Conditions & TFCond_Ubercharged ||
		Conditions & TFCond_IgnoreStates)
		return true;
	player_info_t pInfo;
	if (!i::engine->GetPlayerInfo(pPlayer->GetIndex(), &pInfo))
		return true;
	int nIndex = pPlayer->GetIndex();
	
	if (gEntityCache.Friends[nIndex] && Vars::PriorityOptions::IgnoreFriends) // if the guy is our friend and if we have IgnoreFriends on, return true.
	{
		return true; 
	}
	
	if (gMarkingSystem.IsMarked(pInfo.friendsID) == "IGNORE") 
		return true;
	if (Conditions & TFCond_Cloaked && Vars::PriorityOptions::IgnoreCloaked)
	{
		return true;
	}
}

std::string CPrioritySystem::ShouldBeRage(int playerid)
{
	std::string thisthing = gMarkingSystem.IsMarked(playerid); // friendsID

	return thisthing;
}