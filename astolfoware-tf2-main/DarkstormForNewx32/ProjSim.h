#pragma once
#include "SDK.h"
#include <map>
#include <deque>

enum projectile_type_t {
	TF_PROJECTILE_NONE,
	TF_PROJECTILE_BULLET,
	TF_PROJECTILE_ROCKET,
	TF_PROJECTILE_PIPEBOMB,
	TF_PROJECTILE_PIPEBOMB_REMOTE,
	TF_PROJECTILE_SYRINGE,
	TF_PROJECTILE_FLARE,
	TF_PROJECTILE_JAR,
	TF_PROJECTILE_ARROW,
	TF_PROJECTILE_FLAME_ROCKET,
	TF_PROJECTILE_JAR_MILK,
	TF_PROJECTILE_HEALING_BOLT,
	TF_PROJECTILE_ENERGY_BALL,
	TF_PROJECTILE_ENERGY_RING,
	TF_PROJECTILE_PIPEBOMB_PRACTICE,
	TF_PROJECTILE_CLEAVER,
	TF_PROJECTILE_STICKY_BALL,
	TF_PROJECTILE_CANNONBALL,
	TF_PROJECTILE_BUILDING_REPAIR_BOLT,
	TF_PROJECTILE_FESTIVE_ARROW,
	TF_PROJECTILE_THROWABLE,
	TF_PROJECTILE_SPELL,
	TF_PROJECTILE_FESTIVE_JAR,
	TF_PROJECTILE_FESTIVE_HEALING_BOLT,
	TF_PROJECTILE_BREADMONSTER_JARATE,
	TF_PROJECTILE_BREADMONSTER_MADMILK,
	TF_PROJECTILE_GRAPPLINGHOOK,
	TF_PROJECTILE_SENTRY_ROCKET,
	TF_PROJECTILE_BREAD_MONSTER,
	TF_PROJECTILE_JAR_GAS,
	TF_PROJECTILE_BALLOFFIRE,
	TF_NUM_PROJECTILES
};

struct projectile_info_t {
	projectile_type_t m_type = { };
	Vector m_pos{};
	Vector m_ang{};
	Vector m_hull{};

	float m_velocity{};
	float m_gravity{};

	bool no_spin{};

	float m_lifetime = 10.f;

	std::deque<std::pair<Vector, Vector>> PredictionLines;
};

class c_projectile_sim {
	void GetProjectileFireSetup(CBaseEntity* player, const Vector& ang_in, Vector offset, Vector& pos_out, Vector& ang_out, bool pipes = false, bool interp = false);

	const objectparams_t g_PhysDefaultObjectParams =
	{
		NULL,
		1.0, //mass
		1.0, // inertia
		0.1f, // damping
		0.1f, // rotdamping
		0.05f, // rotIntertiaLimit
		"DEFAULT",
		NULL,// game data
		0.f, // volume (leave 0 if you don't have one or call physcollision->CollideVolume() to compute it)
		1.0f, // drag coefficient
		true,// enable collisions?
	};

public:
	bool GetInfo(CBaseEntity* player, CBaseCombatWeapon* pWeapon, const Vector& vAngles, projectile_info_t& out, bool bQuick = false);
	bool Initialize(const projectile_info_t& info);
	void RunTick(projectile_info_t& info);
	Vector GetOrigin();
	Vector GetVelocity();
};

inline c_projectile_sim gProjSim;