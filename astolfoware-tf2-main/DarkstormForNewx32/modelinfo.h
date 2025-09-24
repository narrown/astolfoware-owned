#pragma once
#include "Entities.h"
#include "Vector.h"
#include "SDKClasses.h"
#include "Memory.h"

class __declspec(align(16))VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};

	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};
struct Ray_t
{
	VectorAligned   m_Start;
	VectorAligned   m_Delta;
	VectorAligned   m_StartOffset;
	VectorAligned   m_Extents;

	bool    m_IsRay;
	bool    m_IsSwept;

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector& start, Vector& end, Vector& mins, Vector& maxs)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start - m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};


struct cplane_t
{
	Vector normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};

struct csurface_t
{
	const char* name;
	short			surfaceProps;
	unsigned short	flags;
};


class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;

	CBaseEntity* m_pEnt;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& vOther);
};
typedef CGameTrace trace_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		CBaseEntity* pEntity = (CBaseEntity*)pEntityHandle;

		switch (pEntity->GetClientClass()->iClassID)
		{
		case 55: // Portal Window
		case 64: // Spawn Door visualizers
		case 117: // Sniper Dots
		case 225: // Medigun Shield
			return false;
			break;
		}

		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CTraceFilterProjectile : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int nContentsMask) {
		CBaseEntity* pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);

		switch (pEntity->GetClientClass()->iClassID)
		{
		case CFuncAreaPortalWindow:
		case CFuncRespawnRoomVisualizer:
		case CSniperDot:
		case CTFReviveMarker:
		case CTFProjectile_Rocket:
		case CTFProjectile_SentryRocket:
		case CTFGrenadePipebombProjectile:
		case CTFProjectile_Jar:
		case CTFProjectile_JarGas:
		case CTFProjectile_JarMilk:
		case CTFProjectile_Throwable:
		case CTFProjectile_ThrowableBreadMonster:
		case CTFStunBall:
		case CTFBall_Ornament:
		case CTFProjectile_Cleaver:
		case CTFProjectile_Arrow:
		case CTFProjectile_HealingBolt:
		case CTFProjectile_Flare:
		case CTFProjectile_BallOfFire:
		case CTFProjectile_GrapplingHook:
		case CTFProjectile_EnergyBall:
		case CTFProjectile_EnergyRing:
		case CTFProjectile_MechanicalArmOrb:
		case CTFProjectile_SpellKartBats:
		case CTFProjectile_SpellKartOrb:
		case CTFProjectile_SpellLightningOrb:
		case CTFProjectile_SpellTransposeTeleport:
		case CTFProjectile_SpellMeteorShower:
		case CTFProjectile_SpellSpawnBoss:
		case CTFProjectile_SpellMirv:
		case CTFProjectile_SpellPumpkin:
		case CTFProjectile_SpellSpawnHorde:
		case CTFProjectile_SpellSpawnZombie:
		case CTFProjectile_SpellBats:
		case CTFProjectile_SpellFireball:
			return false;
		}

		return pEntityHandle != pSkip;
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class IEngineTrace
{
public:	  //We really only need this I guess...
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)//5
	{
		typedef void(__thiscall* TraceRayFn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		return Memory::CallVirtual<TraceRayFn>(this, 4)(this, ray, fMask, pTraceFilter, pTrace);
	}
};
#include "studio.h"
class IVModelInfo
{
public:
	DWORD* GetModel(int index)
	{
		typedef DWORD* (__thiscall* GetModelFn)(void*, int);
		return Memory::CallVirtual<GetModelFn>(this, 1)(this, index);
	}

	int	GetModelIndex(const char* name)
	{
		typedef int(__thiscall* GetModelIndexFn)(void*, const char*);
		return Memory::CallVirtual< GetModelIndexFn >(this, 2)(this, name);
	}

	const char* GetModelName(const DWORD* model)
	{
		typedef const char* (__thiscall* GetModelNameFn)(void*, const DWORD*);
		return Memory::CallVirtual< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const DWORD* mod)
	{
		typedef studiohdr_t* (__thiscall* GetStudiomodelFn)(void*, const DWORD*);
		return Memory::CallVirtual< GetStudiomodelFn >(this, 28)(this, mod);
	}
};