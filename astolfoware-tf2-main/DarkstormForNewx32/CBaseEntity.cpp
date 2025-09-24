#include "SDK.h"

int CBaseEntity::GetHealth()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
}

int CBaseEntity::GetTeamNum()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
}

int& CBaseEntity::GetFlags()
{
	//DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
	static int offset = gNetVars.get_offset("DT_BasePlayer", "m_fFlags");
	return *(int*)(this + offset);
}


BYTE CBaseEntity::GetLifeState()
{
	DYNVAR_RETURN(BYTE, this, "DT_BasePlayer", "m_lifeState");
}

int CBaseEntity::GetClassNum()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
}

Vector CBaseEntity::GetEyeAngles() {
	DYNVAR_RETURN(Vector, this, "DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
}

void CBaseEntity::SetEyeAngles(Vector eye_angle) {
	*(Vector*)(this + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]")) = eye_angle;
}
int CBaseEntity::GetCond()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
}
Vector CBaseEntity::GetCollideableMins()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
}

Vector CBaseEntity::GetCollideableMaxs()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
}

Vector CBaseEntity::GetEyePosition()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + this->GetAbsOrigin();
}

Vector CBaseEntity::GetAbsEyePosition()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}
bool CBaseEntity::IsAlive()
{
	return this->GetLifeState() == LIFE_ALIVE;
}
void CBaseEntity::ForceTauntCam(bool t) // From Dumpster fire
{
	DYNVAR(n, bool, "DT_TFPlayer", "m_nForceTauntCam");
	return n.SetValue(this, t);
}
void CBaseEntity::SetTickBase(int tickbase)
{
	DYNVAR(n, int, "DT_BasePlayer", "localdata", "m_nTickBase");
	return n.SetValue(this, tickbase);
}
Vector CBaseEntity::m_vecOrigin()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_vecOrigin");
}
Vector CBaseEntity::m_vecVelocity()
{
	//DYNVAR_RETURN(Vector, this, "DT_TFPlayer", "m_vecVelocity[0]");
	return Vector(0,0,0);
}
Vector CBaseEntity::m_vecBaseVelocity()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "m_vecBaseVelocity");
}
Vector CBaseEntity::m_vecViewOffset()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}
int CBaseEntity::m_hGroundEntity()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_hGroundEntity");
}
int& CBaseEntity::GetTickBase()
{
	//DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_nTickBase");
	static int offset = gNetVars.get_offset("DT_BasePlayer", "localdata", "m_nTickBase");
	return *(int*)(this + offset);
}
void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}

Vector CBaseEntity::GetHitboxPosition(int iHitbox)
{
	DWORD* model = this->GetModel();
	if (!model)
		return Vector();

	studiohdr_t* hdr = i::modelinfo->GetStudiomodel(model);
	if (!hdr)
		return Vector();

	matrix3x4 matrix[128];
	if (!this->SetupBones(matrix, 128, 0x100, 0))
		return Vector();

	int HitboxSetIndex = *(int*)((DWORD)hdr + 0xB0);
	if (!HitboxSetIndex)
		return Vector();

	mstudiohitboxset_t* pSet = (mstudiohitboxset_t*)(((PBYTE)hdr) + HitboxSetIndex);

	mstudiobbox_t* box = pSet->pHitbox(iHitbox);
	if (!box)
		return Vector();

	Vector vCenter = (box->bbmin + box->bbmax) * 0.5f;

	Vector vHitbox;

	VectorTransform(vCenter, matrix[box->bone], vHitbox);

	return vHitbox;
}
int CBaseEntity::GetHitboxSet()
{
	static int m_nHitboxSet = gNetVars.get_offset("DT_BaseAnimating", "m_nHitboxSet");
	return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_nHitboxSet);
}
int CBaseEntity::GetNumOfHitboxes()
{
	//model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	DWORD* pModel = this->GetModel();
	if (!pModel)
		return 0;

	studiohdr_t* pHdr = (studiohdr_t*)i::modelinfo->GetStudiomodel(pModel);
	if (!pHdr)
		return 0;
	int HitboxSetIndex = *(int*)((DWORD)pHdr + 0xB0);
	if (!HitboxSetIndex)
		return 0;

	mstudiohitboxset_t* pSet = (mstudiohitboxset_t*)(((PBYTE)pHdr) + HitboxSetIndex);
	if (!pSet)
		return 0;

	return pSet->numhitboxes;
}

CBaseCombatWeapon* CBaseEntity::GetActiveWeapon()
{
	DYNVAR(pHandle, DWORD, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (CBaseCombatWeapon*)i::entlist->GetClientEntityFromHandle(pHandle.GetValue(this));
}

void CBaseEntity::CalcAbsouluteVelocity() {
	// 55 8B EC 83 EC ? 56 8B F1 F7 86
	static DWORD signature = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC ? 56 8B F1 F7 86");
	return reinterpret_cast<void(__thiscall*)(void*)>(signature)(this);
}
Vector CBaseEntity::GetAbsVelocity()
{
	CalcAbsouluteVelocity();
	return GetVelocity();
}
const char* CBaseEntity::szGetClass()
{
	DYNVAR(iClass, int, "DT_TFPlayer", "m_PlayerClass", "m_iClass");

	switch (iClass.GetValue(this))
	{
	case TF2_Scout:
		return "Scout";
	case TF2_Soldier:
		return "Soldier";
	case TF2_Pyro:
		return "Pyro";
	case TF2_Demoman:
		return "Demoman";
	case TF2_Heavy:
		return "Heavy";
	case TF2_Engineer:
		return "Engineer";
	case TF2_Medic:
		return "Medic";
	case TF2_Sniper:
		return "Sniper";
	case TF2_Spy:
		return "Spy";
	default:
		return "Unknown class";
	}

	return "Unknown class"; //Just in case
}

int CBaseEntity::GetMaxHealth()
{
	typedef int(__thiscall* OriginalFn)(void*);
	return Memory::CallVirtual<OriginalFn>(this, 107)(this);
}

#include "SignatureManager.h"
float CBaseEntity::CalculateMaxSpeed(bool bIgnoreSpecialAbility)
{
	
	//static DWORD Signature = Memory::find_signature<DWORD>("client.dll", "55 8B EC 83 EC 18 83 3D ? ? ? ? ? 56");
	return reinterpret_cast<float(__thiscall*)(void*, bool)>(sigs::calculatemaxspeed)(this, bIgnoreSpecialAbility);
	
	//return reinterpret_cast<float(__thiscall*)(void*, bool)>(gSignatures.m_dwCalculateMaxSpeed)(this, bIgnoreSpecialAbility);
}

bool CBaseEntity::IsOnGround()
{
	return (this->GetFlags() & FL_ONGROUND);
}
bool CBaseEntity::IsCritBoosted()
{
	return this->GetCond() & TF_COND_CRITBOOSTED
		|| this->GetCond() & TF_COND_CRITBOOSTED_BONUS_TIME
		|| this->GetCond() & TF_COND_CRITBOOSTED_CARD_EFFECT
		|| this->GetCond() & TF_COND_CRITBOOSTED_CTF_CAPTURE
		|| this->GetCond() & TF_COND_CRITBOOSTED_FIRST_BLOOD
		|| this->GetCond() & TF_COND_CRITBOOSTED_ON_KILL
		|| this->GetCond() & TF_COND_CRITBOOSTED_PUMPKIN
		|| this->GetCond() & TF_COND_CRITBOOSTED_RAGE_BUFF
		|| this->GetCond() & TF_COND_CRITBOOSTED_RUNE_TEMP
		|| this->GetCond() & TF_COND_CRITBOOSTED_USER_BUFF;
}

bool CBaseEntity::IsMiniCritBoosted()
{
	return this->GetCond() & TF_COND_MINICRITBOOSTED_ON_KILL
		|| this->GetCond() & TF_COND_NOHEALINGDAMAGEBUFF
		|| this->GetCond() & TF_COND_ENERGY_BUFF
		/*|| InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE)*/;
}

bool CBaseCombatWeapon::CalcIsAttackCriticalHelperMelee() {
	// new sig : 55 8B EC A1 ? ? ? ? 83 EC ? 83 78 ? ? 57
	// old sig : 55 8B EC A1 ? ? ? ? 83 EC 08 83 78 30 00 57
	return reinterpret_cast<bool(__thiscall*)(void*)>(sigs::calcisattackcriticalhelpermelee)(this);
}

bool CBaseCombatWeapon::CalcIsAttackCriticalHelper() {
	return reinterpret_cast<bool(__thiscall*)(void*)>(sigs::calcisattackcriticalhelper)(this);
}

int CBaseEntity::GetCondEx2()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCondEx2");
}
Vector CBaseEntity::GetVelocity()
{
	static auto FN = reinterpret_cast<void(__thiscall*)(CBaseEntity*, Vector&)>(sigs::estimateabsvelocity);
	Vector v;
	FN(this, v);
	return v;
}

bool CBaseCombatWeapon::can_shoot(CBaseEntity* pLocal)
{

	if (!pLocal || !pLocal->IsAlive() || pLocal->GetCond() & TFCond_Taunting || pLocal->GetCond() & TFCond_Cloaked)
		return false;

	// C_TFWeaponFlameBall::PrimaryAttack
	if (GetWeaponID() == TF_WEAPON_FLAME_BALL)
		return (pLocal->m_tank_pressure() >= 100.0f);

	if (ammo() > 0 && is_reloading())
	{
		return true;
	}

	return m_next_primary_attack() < TICKS_TO_TIME(pLocal->GetTickBase());
}
// todo move these sigs to sig manager, or get rid of these functions.
// we dont use these.
bool CBaseEntity::PhysicsRunThink(const thinkmethods_t thinkMethod)
{
	static DWORD Nigger = Memory::find_signature<DWORD>("client.dll", "55 8B EC 53 8B D9 56 57 8B 83 ? ? ? ? C1 E8 16 A8 01 B0 01 0F 85 ? ? ? ?");
	return reinterpret_cast<bool(__thiscall*)(void*, thinkmethods_t)>(Nigger)(this, thinkMethod);
}

int CBaseEntity::SaveData(const char* const context, const int slot, const int type)
{
	static DWORD SaveNiggers = Memory::find_signature<DWORD>("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 57 8B 7D");
	return reinterpret_cast<int(__thiscall*)(void*, const char*, int, int)>(SaveNiggers)(this, context, slot, type);
}

bool CBaseEntity::UsingStandardWeaponsInVehicle()
{
	// 	GETPATT(m_dwUsingStandardWeaponsInVehicle, "client.dll", "56 57 8B F9 8B 97 ? ? ? ? 85 D2 74 52");
	static DWORD Sig = Memory::find_signature<DWORD>("client.dll", "56 57 8B F9 8B 97 ? ? ? ? 85 D2 74 52");
	static auto FN = reinterpret_cast<bool(__thiscall*)(void*)>(Sig);
	return FN(this);
}

void CBaseEntity::UpdateButtonState(const int nUserCmdButtonMask)
{
	// GETPATT(m_dwUpdateButtonState, "client.dll", "55 8B EC 8B 55 08 56 8B B1");
	static DWORD button = Memory::find_signature<DWORD>("client.dll", "55 8B EC 8B 55 08 56 8B B1");
	
	return reinterpret_cast<void(__thiscall*)(void*, int)>(button)(this, nUserCmdButtonMask);
}
int CBaseEntity::RestoreData(const char* const context, const int slot, const int type)
{
	// MAKE_SIGNATURE( C_BaseEntity_RestoreData, "client.dll", "55 8B EC 83 EC ? 53 56 57 8B 7D ? 8B D9 83 FF ? 75", 0 );
														     // 55 8B EC 83 EC ? 53 56 57 8B 7D ? 8B D9 83 FF ? 75
	static DWORD RestoreTND = gSignature.GetClientSignature("55 8B EC 83 EC ? 53 56 57 8B 7D ? 8B D9 83 FF ? 75");
	
	return reinterpret_cast<int(__thiscall*)(const char*, int, int)>(RestoreTND)(context, slot, type);
}
void CBaseEntity::SelectItem(const char* ptr, int subtype)
{
	return Memory::CallVirtual<void(__thiscall*)(void*, const char*, int)>(this, 270)(this, ptr, subtype);
}

bool CClientState::IsPaused()
{
	return reinterpret_cast<bool(__thiscall*)(void*)>(sigs::is_paused)(this);
}

CBaseCombatWeapon* C_BasePlayer::GetWeaponFromSlot(int slot)
{
	static DWORD MyWeapons = gNetVars.GetNetVar("CBaseCombatCharacter", "m_hMyWeapons");
	int Weapon = *reinterpret_cast<int*>(this + (MyWeapons + (slot * 0x4)));
	return i::entlist->GetClientEntityFromHandle(Weapon)->as<CBaseCombatWeapon>();
}
bool KeyValues::LoadFromBuffer(char const* resource_name, const char* buffer, void* file_system, const char* path_id)
{
	using fn = int(__thiscall*)(KeyValues*, char const*, const char*, void*, const char*);
	static fn FN = reinterpret_cast<fn>(sigs::key_values_loadfrombuffer);
	return FN(this, resource_name, buffer, file_system, path_id);
}
IMaterial* IMaterialSystem::CreateMaterialSigged(const char* pMaterialName, KeyValues* pVMTKeyValues) {
	//  55 8B EC 56 57 FF 75 ? 8B F9 68
	// materialsystem.dll / MaterialSystem.dll
	typedef IMaterial* (__thiscall* fn)(PVOID, const char*, KeyValues*);
	static fn FN = reinterpret_cast<fn>(sigs::create_material_sig);
	return FN(this, pMaterialName, pVMTKeyValues);
}
void KeyValues::Initialize(char* name)
{
	using fn = KeyValues * (__thiscall*)(KeyValues*, char*);
	static fn FN = reinterpret_cast<fn>(sigs::key_values_init - 0x42);
	FN(this, name);
}

KeyValues::KeyValues(const char* name)
{
	char _name[128];
	sprintf_s(_name, sizeof(_name), name);
	this->Initialize((char*)_name);
}

void ISurface::StartDrawing()
{
	typedef void(__thiscall* start_drawing_fn)(ISurface*);								         //55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14
	static auto start_drawing_ = reinterpret_cast<start_drawing_fn>(sigs::start_drawing);//Memory::find_signature< start_drawing_fn >("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
	return start_drawing_(this);
}

void ISurface::FinishDrawing()
{
	typedef void(__thiscall* finish_drawing_fn)(ISurface*);										  // 55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00
	static auto finish_drawing_ = reinterpret_cast<finish_drawing_fn>(sigs::finish_drawing);//Memory::find_signature< finish_drawing_fn >("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");
	return finish_drawing_(this);
}

int INetChannel::SendDatagram(bf_write* write) {
	// "engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9"
	typedef int(__thiscall* senddatagram)(PVOID, bf_write*); // ecx is void, theres no EDX, so we just skip right to bf_write param.
	static auto senddatagramtwo = reinterpret_cast<senddatagram>(sigs::send_datagram);//Memory::find_signature<senddatagram>("engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9");
	return senddatagramtwo(this, write);
}