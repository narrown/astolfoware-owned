#pragma once
#include "CNetVars.h"
#include "Memory.h"
#include "Vector.h"
#include "Enums.h"
#include "SDKClasses.h"
#include "matrix3x4.h"
#include "ClientClass.h"
#include "CUserCmd.h"




#define NETVAR_OFF(_name, type, table, name, offset) inline type& _name() \
{ \
	static int nOffset = gNetVars.GetNetVar(table, name) + offset; \
	return *reinterpret_cast<type*>(reinterpret_cast<DWORD>(this) + nOffset); \
}
#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static int offset = gNetVars.GetNetVar(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + offset); \
}

#define OFFSETThing( name, type, offset ) inline type& name( ) { \
	return *reinterpret_cast< type* >( reinterpret_cast< uint32_t >( this ) + offset ); \
}

class CBaseCombatWeapon;


class IClientEntity
{
public:
	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 9)(this);
	}
	Vector& GetAbsAngles()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 10)(this);
	}

public: // renderable
	__inline void* Renderable() { return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(this) + 0x4)); }
	DWORD* GetModel()
	{
		typedef DWORD* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(Renderable(), 9)(Renderable());
	}
	void DrawModel(int flags) {
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return Memory::CallVirtual<OriginalFn>(Renderable(), 10)(Renderable(), flags);
	}
	bool SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4*, int, int, float);
		return Memory::CallVirtual<OriginalFn>(Renderable(), 16)(Renderable(), pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, Vector&, Vector&);
		Memory::CallVirtual<OriginalFn>(Renderable(), 20)(Renderable(), mins, maxs);
	}
	matrix3x4& GetRgflCoordinateFrame()
	{
		//PVOID pRenderable = static_cast<PVOID>(this + 0x4);
		typedef matrix3x4& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(Renderable(), 34)(Renderable());
	}
public: // networkable
	__inline void* Networkable() { return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(this) + 0x8)); }
	ClientClass* GetClientClass()
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(Networkable(), 2)(Networkable());
	}
	bool IsDormant()
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(Networkable(), 8)(Networkable());
	}
	int GetIndex()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(Networkable(), 9)(Networkable());
	}
	template < typename T >
	inline T* as() {
		return reinterpret_cast<T*>(this);
	}
};

class CBaseEntity : public IClientEntity
{
public:
	void GetWorldSpaceCenter(Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds(vMin, vMax);
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	void SetCurrentCmd(CUserCmd* cmd)
	{
		DYNVAR_SET(CUserCmd*, (this - 0x4), cmd, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
	}
	Vector GetAbsVelocity();
	void CalcAbsouluteVelocity();
	int GetHealth();
	int GetTeamNum();
	int& GetFlags();
	BYTE GetLifeState();
	int GetClassNum();
	const char* szGetClass();
	int GetCond();
	CBaseCombatWeapon* GetActiveWeapon();
	Vector GetCollideableMins();
	Vector GetCollideableMaxs();
	Vector GetEyePosition(); //yeah i know its actually view offset, but nobody will notice since you guys are just here to paste
	Vector GetAbsEyePosition(); //Without origin added, although you could just subtract origin, tbh idk why i have this
	Vector GetVelocity();
	Vector GetEyeAngles();
	void SetEyeAngles(Vector eye_angle);
	bool IsAlive();
	void ForceTauntCam(bool t); // From Dumpster fire
	void SetTickBase(int tickbase);
	Vector m_vecOrigin();
	Vector m_vecVelocity();
	Vector m_vecBaseVelocity();
	Vector m_vecViewOffset();
	int m_hGroundEntity();
	int& GetTickBase();
	Vector GetHitboxPosition(int iHitbox);
	int GetHitboxSet();
	int GetNumOfHitboxes();
	int GetMaxHealth();

	float CalculateMaxSpeed(bool bIgnoreSpecialAbility = false);
	bool IsOnGround();
	Vector GetVecOrigin()
	{
		DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_vecOrigin");
	}
	Vector GetShootPos()
	{
		return GetVecOrigin() + m_vecViewOffset();
	}
	bool IsCritBoosted();
	bool IsMiniCritBoosted();
	int deadflag()
	{
		/* "DT_BasePlayer", "pl", "deadflag" */
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "pl", "deadflag");
	}
	OFFSETThing(GetMoveType, MoveType_t, 0x1A4);
	BYTE GetWaterLevel()
	{
		DYNVAR_RETURN(BYTE, this, "DT_TFPlayer", "m_nWaterLevel");
	}
	bool IsSwimming()
	{
		return (GetWaterLevel() > 1);
	}
	bool IsScoped()
	{
		return (GetCond() & TFCond_Zoomed);
	}

	/* although i have these netvars in darkstorm netvar manager, you cannot use them for movesim.*/

	//NETVAR(m_iCritMult, int, "CTFPlayer", "m_iCritMult");
	bool IsSolid()
	{
		return (GetFlags() & FL_ONGROUND);
	}
	void* GetThrower()
	{
		DYNVAR_RETURN(void*, this, "DT_BaseGrenade", "m_hThrower");
	}
	bool IsPlayer()
	{
		return (this->GetClientClass()->iClassID == CTFPlayer);
	}
	bool IsBuilding() {
		return (this->GetClientClass()->iClassID == CObjectDispenser
			|| this->GetClientClass()->iClassID == CObjectSentrygun);
	}
	bool IsCharging()
	{
		return (GetCond() & TFCond_Charging);
	}
	bool IsDucked()
	{
		return (GetFlags() & FL_DUCKING);
	}
	int GetCondEx2();

	OFFSETThing(m_tank_pressure, float, 0x1B40);
	OFFSETThing(PipebombType, int, 0x8FC); // this is still right l0l
	// M_OFFSETGET(VecVelocity, Vec3, 0x120)
	OFFSETThing(VecVelocity, Vector, 0x120);
	float GetSimulationTime()
	{
		DYNVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
	}
	int FlagStatus()
	{
		DYNVAR_RETURN(int, this, "DT_CaptureFlag", "m_nFlagStatus");
	}
	NETVAR(m_thrower, int, "CBaseGrenade", "m_hThrower");
	NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack");


	void setfov(int fov)
	{
		//DYNVAR(n, int, "DT_BasePlayer", "m_iFOV");
		//DYNVAR(n2, int, "DT_BasePlayer", "m_iDefaultFOV");
		//n2.SetValue(this, fov);
		//n2.SetValue(this, fov);
		DYNVAR_SET(int, this, fov, "DT_BasePlayer", "m_iFOV");
	}
	bool PhysicsRunThink(const thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS);
	int SaveData(const char* const context, const int slot, const int type);
	void SelectItem(const char* ptr, int subtype);
	bool UsingStandardWeaponsInVehicle();
	void UpdateButtonState(const int nUserCmdButtonMask);
	int RestoreData(const char* const context, const int slot, const int type);

};
class C_BasePlayer : public CBaseEntity
{
public:
	NETVAR(m_origin, Vector, "CBaseEntity", "m_vecOrigin");
	NETVAR(m_velocity, Vector, "CBasePlayer", "m_vecVelocity[0]");
	NETVAR(m_base_velocity, Vector, "CBasePlayer", "m_vecBaseVelocity");
	NETVAR(m_ground_entity, int, "CBasePlayer", "m_hGroundEntity");
	NETVAR(m_flags, int, "CBasePlayer", "m_fFlags");
	NETVAR(m_in_duck_jump, bool, "CBasePlayer", "m_bInDuckJump");
	NETVAR(m_view_offset, Vector, "CBasePlayer", "m_vecViewOffset[0]");
	NETVAR(m_duck_time, float, "CBasePlayer", "m_flDucktime");
	NETVAR(m_duck_jump_time, float, "CBasePlayer", "m_flDuckJumpTime");
	NETVAR(m_ducked, bool, "CBasePlayer", "m_bDucked");
	NETVAR(m_ducking, bool, "CBasePlayer", "m_bDucking");
	NETVAR(m_model_scale, float, "CBaseAnimating", "m_flModelScale");
	NETVAR(m_initial_velocity, Vector, "CTFWeaponBaseGrenadeProj", "m_vInitialVelocity");
	NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
	NETVAR(m_hVehicle, int, "CBasePlayer", "m_hVehicle");
	NETVAR(m_damage_done, int, "CTFPlayer", "m_iDamageDone");
	NETVAR(m_criticals, int, "CTFPlayer", "m_iCrits");
	NETVAR(m_headshots, int, "CTFPlayer", "m_iHeadshots");
	NETVAR(m_crit_mult, int, "CTFPlayer", "m_iCritMult");
	NETVAR(GetObserverTarget, int, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(GetObserverMode, int, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(m_cycle, float, "CBaseAnimating", "m_flCycle");
	NETVAR(m_fov, int, "CBasePlayer", "m_iFOV");
	NETVAR(FallVelocity, float, "CBasePlayer", "m_flFallVelocity");
	NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed");
	NETVAR(m_water_level, BYTE, "CTFPlayer", "m_nWaterLevel");
	NETVAR(NextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
	NETVAR(ObserverTarget, int, "CBasePlayer", "m_hObserverTarget");
	NETVAR(ObserverMode, int, "CBasePlayer", "m_iObserverMode");
	inline int& m_nImpulse()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x122C);
	}
	void PostThink()
	{
		return Memory::CallVirtual<void(__thiscall*)(C_BasePlayer*)>(this, 261)(this);
	}

	void PreThink()
	{
		return Memory::CallVirtual<void(__thiscall*)(C_BasePlayer*)>(this, 260)(this);
	}
	void Think()
	{
		return Memory::CallVirtual<void(__thiscall*)(C_BasePlayer*)>(this, 174)(this);
	}
	NETVAR(m_bJumping, bool, "CTFPlayer", "m_bJumping");
	NETVAR(m_vecPunchAngle, Vector, "CBasePlayer", "m_vecPunchAngle");
	NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity");
	NETVAR(m_vecPunchAngleVel, Vector, "CBasePlayer", "m_vecPunchAngleVel");
	NETVAR(m_nPlayerCond, int, "CTFPlayer", "m_nPlayerCond");
	NETVAR(m_nPlayerCondEx, int, "CTFPlayer", "m_nPlayerCondEx");
	NETVAR(m_nPlayerCondEx4, int, "CTFPlayer", "m_nPlayerCondEx4");
	NETVAR(_condition_bits, int, "CTFPlayer", "_condition_bits");
	NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime");
	NETVAR(m_hConstraintEntity, int, "CBasePlayer", "m_hConstraintEntity");
	NETVAR(m_vecConstraintCenter, Vector, "CBasePlayer", "m_vecConstraintCenter");
	NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius");
	NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth");
	NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
	NETVAR(m_nPlayerCondEx3, int, "CTFPlayer", "m_nPlayerCondEx3");
	NETVAR(m_nPlayerCondEx2, int, "CTFPlayer", "m_nPlayerCondEx2");
	NETVAR(m_iCritMult, int, "CTFPlayer", "m_iCritMult");
	inline int& m_nButtons()
	{
		static const int nOffset = (gNetVars.GetNetVar("CBasePlayer", "m_hConstraintEntity") - 0x8);
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	CBaseCombatWeapon* GetWeaponFromSlot(int slot);
};

class CObjectClass : public CBaseEntity
{
public:
	inline int GetLevel()
	{
		static int m_iUpgradeLevel = gNetVars.get_offset("DT_BaseObject", "m_iUpgradeLevel");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iUpgradeLevel);
	}

	inline bool IsSapped()
	{
		static int m_bHasSapper = gNetVars.get_offset("DT_BaseObject", "m_bHasSapper");
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + m_bHasSapper);
	}

	inline bool IsBuilding()
	{
		static int m_bBuilding = gNetVars.get_offset("DT_BaseObject", "m_bBuilding");
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + m_bBuilding);
	}

	//m_iObjectMode
	inline int GetObjectMode()
	{
		static int m_iObjectMode = gNetVars.get_offset("DT_BaseObject", "m_iObjectMode");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iObjectMode);
	}

	inline bool IsBeingCarried()
	{
		static int m_bCarried = gNetVars.get_offset("DT_BaseObject", "m_bCarried");
		static int m_bCarryDeploy = gNetVars.get_offset("DT_BaseObject", "m_bCarryDeploy");
		static int m_bPlacing = gNetVars.get_offset("DT_BaseObject", "m_bPlacing");

		if (*reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + m_bCarried)
			|| *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + m_bCarryDeploy)
			|| *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + m_bPlacing))
			return true;

		return false;
	}

	inline float GetPercentageConstructed()
	{
		static int m_flPercentageConstructed = gNetVars.get_offset("DT_BaseObject", "m_flPercentageConstructed");

		if (IsBuilding())
			return *reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + m_flPercentageConstructed);
		else
			return NULL;
	}

	inline int GetHealth()
	{
		static int m_iHealth = gNetVars.get_offset("DT_BaseObject", "m_iHealth");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iHealth);
	}

	inline int GetMaxHealth()
	{
		static int m_iHealth = gNetVars.get_offset("DT_BaseObject", "m_iMaxHealth");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iHealth);
	}


	inline int GetUpgradedMetal()
	{
		static int m_iUpgradeMetal = gNetVars.get_offset("DT_BaseObject", "m_iUpgradeMetal");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iUpgradeMetal);
	}

	inline int GetState()
	{
		static int m_iState = gNetVars.get_offset("DT_BaseObject", "m_iState");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iState);
	}

	inline int GetTeamNum()
	{
		static int m_iTeamNum = gNetVars.get_offset("DT_BaseEntity", "m_iTeamNum");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iTeamNum);
	}
};

class CTFObjectSentryGun : public CObjectClass
{
public:

	inline int GetRocket()
	{
		static int m_iAmmoRockets = gNetVars.get_offset("DT_ObjectSentrygun", "m_iAmmoRockets");

		if (GetLevel() == 3)
			return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iAmmoRockets);

		return NULL;
	}

	inline int GetAmmo()
	{
		static int m_iAmmoShells = gNetVars.get_offset("DT_ObjectSentrygun", "m_iAmmoShells");
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_iAmmoShells);
	}

	inline const char* GetStateString()
	{
		switch (GetState())
		{
		case 1:
		{
			return "Idle";
		}
		case 2:
		{
			return "Attacking";
		}
		case 3:
		{
			return "Upgrading";
		}
		}
		return "Unknown";
	}
};

class CObjectDispenserClass : public CObjectClass
{
public:
	inline int GetMetalReserve()
	{
		DYNVAR_RETURN(int, this, "DT_ObjectDispenser", "m_iAmmoMetal");
	}
};
#include "SignatureManager.h"
class CBaseCombatWeapon : public CBaseEntity
{
public:
	NETVAR(m_flLastCritCheckTime, float, "CTFWeaponBase", "m_flLastCritCheckTime");
	NETVAR(m_nViewModelIndex, float, "CBaseCombatWeapon", "m_nViewModelIndex");
	NETVAR_OFF(m_crit_token_bucket, float, "CBaseCombatWeapon", "m_nViewModelIndex", -12);
	NETVAR_OFF(m_last_rapid_fire_crit_check_time, float, "CTFWeaponBase", "m_flLastCritCheckTime", +12);
	OFFSETThing(m_weapon_mode, int, 0x0B24); // no idea what the fuck this is..
	NETVAR(hHealingTarget, unsigned int, "CWeaponMedigun", "m_hHealingTarget");
	NETVAR(ChargeResistType, int, "CWeaponMedigun", "m_nChargeResistType");
	NETVAR(ChargeLevel, float, "CWeaponMedigun", "m_flChargeLevel");
	NETVAR(DetonateTime, float, "CWeaponGrenadeLauncher", "m_flDetonateTime");
	int GetMaxClip1()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 318)(this);
	}

	int GetMaxClip2()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 319)(this);
	}

	int GetSlot()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 330)(this);
	}

	char* GetName()
	{
		typedef char* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 333)(this);
	}

	char* GetPrintName()
	{
		typedef char* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 330)(this);
	}
	bool calc_is_attack_critical() { /* ??? returns to nullptr?? what the fuck? */
		typedef bool(__thiscall* calc_is_attack_critical_fn)(CBaseCombatWeapon*);
		return Memory::CallVirtual< calc_is_attack_critical_fn >(this, 396)(this);
	}

	bool can_fire_random_critical_shot(float crit_chance) {
		typedef bool(__thiscall* can_fire_random_critical_shot_fn)(CBaseCombatWeapon*, float);
		return Memory::CallVirtual< can_fire_random_critical_shot_fn >(this, 426)(this, crit_chance);
	}

	int GetWeaponID()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 383)(this);
	}
	int GetItemDefinitionIndex()
	{
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}
	int m_iClip1()
	{
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "m_iClip1");
	}
	NETVAR(Clip1, int, "CBaseCombatWeapon", "LocalWeaponData", "m_iClip1")
		NETVAR(Clip2, int, "CBaseCombatWeapon", "LocalWeaponData", "m_iClip2")
		float GetChargeBeginTime()
	{
		DYNVAR_RETURN(float, this, "DT_WeaponPipebombLauncher", "PipebombLauncherLocalData", "m_flChargeBeginTime");
	}
	int ammo()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x8BC); // update?
	}
	bool is_reloading()
	{
		static auto next_primary_attack_offset = gNetVars.get_offset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		bool in_reload = *reinterpret_cast<bool*>(this + (next_primary_attack_offset + 0xc));
		int reload_mode = *reinterpret_cast<int*>(this + 0xb28);
		return (in_reload || reload_mode != 0);
	}
	NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack");
	float m_next_primary_attack()
	{
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	}
	float GetSwingRange(CBaseEntity* pLocal) {
		return static_cast<float>(Memory::CallVirtual<int(__thiscall*)(CBaseEntity*)>(this, 451)(pLocal));
	}
	bool CalcIsAttackCriticalHelperMelee();
	bool can_shoot(CBaseEntity* pLocal);
	bool CalcIsAttackCriticalHelper();
	NETVAR(m_flObservedCritChance, float, "CTFWeaponBase", "m_flObservedCritChance");
	bool CanBackStab()
	{
		static int offset = gNetVars.get_offset("DT_TFWeaponKnife", "m_bReadyToBackstab");
		return *(bool*)(this + offset);
	}

};