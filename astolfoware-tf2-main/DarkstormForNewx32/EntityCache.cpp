#include "EntityCache.h"
#include "CConsole.h"
#include "Utils.h"

enum ETFGrenadeProjectileType
{
	TYPE_PIPE,
	TYPE_STICKY
};
void CEntityCache::Fill()
{
	CBaseEntity* _pLocal = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<CBaseEntity>();
	//C_BasePlayer* pLocal = gInts.EntList->GetClientEntitySecond(gInts.Engine->GetLocalPlayer());
	if (_pLocal && _pLocal->GetTeamNum())
	{
		m_pLocal = _pLocal;
		m_pLocalWeapon = m_pLocal->GetActiveWeapon();
		/*
		switch (m_pLocal->GetObserverMode())
		{
		case OBS_MODE_FIRSTPERSON:
		case OBS_MODE_THIRDPERSON:
		{
			m_pObservedTarget = g_Interfaces.EntityList->GetClientEntityFromHandle(m_pLocal->GetObserverTarget());
			break;
		}
		default: break;
		}
		*/
		CBaseEntity* pEntity; int n;
		for (n = 1; n < i::entlist->GetHighestEntityIndex(); n++)
		{
			pEntity = i::entlist->GetClientEntity(n)->as<CBaseEntity>();

			if (!pEntity || pEntity->IsDormant())
				continue;

			if (pEntity == m_pLocal)
			{
				if (!i::input->cam_is_third_person())
					continue;
			}

			auto nClassID = pEntity->GetClientClass()->iClassID;
			switch (nClassID)
			{
			case CTFPlayer:
			{
				m_vecGroups[EGroupType::PLAYERS_ALL].push_back(pEntity->as<C_BasePlayer>());
				m_vecGroups[pEntity->GetTeamNum() != m_pLocal->GetTeamNum() ? EGroupType::PLAYERS_ENEMIES : EGroupType::PLAYERS_TEAMMATES].push_back(pEntity->as<C_BasePlayer>());
				break;
			}

			case CObjectSentrygun:
			{
				
				m_vecGroups[EGroupType::BUILDINGS_ALL].push_back(pEntity);
				m_vecGroups[pEntity->GetTeamNum() != m_pLocal->GetTeamNum() ? EGroupType::BUILDINGS_ENEMIES : EGroupType::BUILDINGS_TEAMMATES].push_back(pEntity);
				break;
			}
			case CObjectDispenser:
			{
				m_vecGroups[EGroupType::BUILDINGS_ALL].push_back(pEntity);
				m_vecGroups[pEntity->GetTeamNum() != m_pLocal->GetTeamNum() ? EGroupType::BUILDINGS_ENEMIES : EGroupType::BUILDINGS_TEAMMATES].push_back(pEntity);
				break;
			}
			/* i just use this for ESP so fuck u nigger */
			case CTFProjectile_Rocket:
			case CTFGrenadePipebombProjectile:
			case CTFProjectile_Jar:
			case CTFProjectile_JarGas:
			case CTFProjectile_JarMilk:
			case CTFProjectile_Arrow:
			case CTFProjectile_SentryRocket:
			case CTFProjectile_Flare:
			case CTFProjectile_GrapplingHook:
			case CTFProjectile_Cleaver:
			case CTFProjectile_EnergyBall:
			case CTFProjectile_EnergyRing:
			case CTFProjectile_HealingBolt:
			case CTFProjectile_ThrowableBreadMonster:
			case CCaptureFlag:
			{
				m_vecGroups[EGroupType::WORLDESP].push_back(pEntity);
				
				if (nClassID == CTFGrenadePipebombProjectile && pEntity->PipebombType() == TYPE_STICKY)
				{
					CBaseEntity* thower = i::entlist->GetClientEntityFromHandle(reinterpret_cast<int>(pEntity->GetThrower()))->as<CBaseEntity>();

					if (thower == m_pLocal)
						m_vecGroups[EGroupType::LOCAL_STICKIES].push_back(pEntity);

					break;
				}
				

				break;
			}
			case CTFAmmoPack:
			{
				m_vecGroups[EGroupType::WORLD_AMMO].push_back(pEntity);
				break;
			}
			
			default: break;
			}
		}

		UpdateFriends();
	}
}




void CEntityCache::UpdateFriends()
{
	static size_t CurSize, OldSize;
	const auto Players = GetGroup(EGroupType::PLAYERS_ALL);
	CurSize = Players.size();

	if (CurSize != OldSize)
	{
		for (const auto& Player : Players)
			Friends[Player->GetIndex()] = Utils::IsPlayerOnSteamFriendList(Player);
	}

	OldSize = CurSize;
}

void CEntityCache::Clear()
{
	m_pLocal = nullptr;
	m_pLocalWeapon = nullptr;
	m_pObservedTarget = nullptr;

	for (auto& Group : m_vecGroups)
		Group.second.clear();
}

const std::vector<CBaseEntity*>& CEntityCache::GetGroup(const EGroupType& Group)
{
	return m_vecGroups[Group];
}