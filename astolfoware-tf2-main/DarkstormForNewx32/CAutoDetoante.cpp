#include "CAutoDet.h"
#include "Utils.h"
enum
{
	PARTITION_ENGINE_SOLID_EDICTS = (1 << 0),		// every edict_t that isn't SOLID_TRIGGER or SOLID_NOT (and static props)
	PARTITION_ENGINE_TRIGGER_EDICTS = (1 << 1),		// every edict_t that IS SOLID_TRIGGER
	PARTITION_CLIENT_SOLID_EDICTS = (1 << 2),
	PARTITION_CLIENT_RESPONSIVE_EDICTS = (1 << 3),		// these are client-side only objects that respond to being forces, etc.
	PARTITION_ENGINE_NON_STATIC_EDICTS = (1 << 4),		// everything in solid & trigger except the static props, includes SOLID_NOTs
	PARTITION_CLIENT_STATIC_PROPS = (1 << 5),
	PARTITION_ENGINE_STATIC_PROPS = (1 << 6),
	PARTITION_CLIENT_NON_STATIC_EDICTS = (1 << 7),		// everything except the static props
};

#define MAX_SPHERE_QUERY	512

class CEntitySphereQuery
{
public:
	CEntitySphereQuery(const Vector& center, const float radius, const int flagMask = 0, const int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS) {
		typedef void(__thiscall* fn)(void*, const Vector&, float, int, int);
		static auto function = reinterpret_cast<fn>(sigs::sphere_thing); 
		function(this, center, radius, flagMask, partitionMask);
	}

	CBaseEntity* GetCurrentEntity() {
		return (m_nListIndex < m_nListCount) ? m_pList[m_nListIndex] : nullptr;
	}

	void NextEntity() {
		m_nListIndex++;
	}

private:
	int m_nListIndex, m_nListCount;
	CBaseEntity* m_pList[MAX_SPHERE_QUERY];
};
#include "Vars.h"
#include "EntityCache.h"
/* set it to false.. crash? */
/* god i FUCKING love c++. */
float m_flRadius = 0; 
bool  m_bDetonated = false;


enum e_tf_grenade_projectile_type {
	TYPE_PIPE, // 0
	TYPE_STICKY // 1
};
void CAutoDetonate::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	if (!Vars::Misc::AutoDetonate)
		return;
	if (!pLocal)
		return;


	m_bDetonated = false;
	m_flRadius = (115.0f * 0.8f);

	auto do_det = false;

	for (const auto& sticky : gEntityCache.GetGroup(EGroupType::WORLDESP)) {

		if (sticky->GetClientClass()->iClassID != CTFGrenadePipebombProjectile || sticky->PipebombType() != TYPE_STICKY)
			continue;
		C_BasePlayer* pBaseEntity = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<C_BasePlayer>();
		if (i::entlist->GetClientEntityFromHandle(reinterpret_cast<int>(sticky->GetThrower())) != pBaseEntity)
			continue;

		CBaseEntity* entity = nullptr;
		Vector StickyWorldSpaceCenter;
		sticky->GetWorldSpaceCenter(StickyWorldSpaceCenter);
		for (CEntitySphereQuery sphere(StickyWorldSpaceCenter, m_flRadius); (entity = sphere.GetCurrentEntity()) != nullptr; sphere.NextEntity()) {
			if (!entity || entity == pBaseEntity || entity->IsDormant() || (!entity->IsPlayer()) || (entity->IsPlayer() && !entity->as< C_BasePlayer >()->IsAlive()) || entity->GetTeamNum() == sticky->GetTeamNum())
				continue;
			Vector StickyWorldSpaceCenter2;
			Vector EntityWSC;
			entity->GetWorldSpaceCenter(EntityWSC);
			sticky->GetWorldSpaceCenter(StickyWorldSpaceCenter2);
			if (!Utils::is_visible(sticky, entity, StickyWorldSpaceCenter2, EntityWSC, MASK_SHOT))
				continue;
			do_det = true;
		}

		if (do_det)
			break;
	}

	if (do_det)
		pCmd->buttons |= IN_ATTACK2;
}