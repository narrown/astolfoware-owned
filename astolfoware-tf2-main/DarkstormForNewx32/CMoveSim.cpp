#include "CMoveSim.h"
#include "SDK.h"
#include "Math.h"
#include "CreateMove.h"

static CUserCmd DummyCmd = {};

//since we're going to call game functions some entity data will be modified (we'll modify it too), we'll have to restore it after running
class CPlayerDataBackup
{
private:
	Vector m_vecOrigin = {};
	Vector m_vecVelocity = {};
	Vector m_vecBaseVelocity = {};
	Vector m_vecViewOffset = {};
	int m_hGroundEntity = 0;
	int m_fFlags = 0;
	float m_flDucktime = 0.0f;
	float m_flDuckJumpTime = 0.0f;
	bool m_bDucked = false;
	bool m_bDucking = false;
	bool m_bInDuckJump = false;
	float m_flModelScale = 0.0f;

public:
	inline void Store(C_BasePlayer* pPlayer)
	{
		m_vecOrigin = pPlayer->m_origin();
		m_vecVelocity = pPlayer->m_velocity();
		m_vecBaseVelocity = pPlayer->m_base_velocity();
		m_vecViewOffset = pPlayer->m_view_offset();
		m_hGroundEntity = pPlayer->m_ground_entity();
		m_fFlags = pPlayer->m_flags();
		m_flDucktime = pPlayer->m_duck_time();
		m_flDuckJumpTime = pPlayer->m_duck_jump_time();
		m_bDucked = pPlayer->m_ducked();
		m_bDucking = pPlayer->m_ducking();
		m_bInDuckJump = pPlayer->m_in_duck_jump();
		m_flModelScale = pPlayer->m_model_scale();
	}

	inline void Restore(C_BasePlayer* pPlayer)
	{
		pPlayer->m_origin() = m_vecOrigin;
		pPlayer->m_velocity() = m_vecVelocity;
		pPlayer->m_base_velocity() = m_vecBaseVelocity;
		pPlayer->m_view_offset() = m_vecViewOffset;
		pPlayer->m_ground_entity() = m_hGroundEntity;
		pPlayer->m_flags() = m_fFlags;
		pPlayer->m_duck_time() = m_flDucktime;
		pPlayer->m_duck_jump_time() = m_flDuckJumpTime;
		pPlayer->m_ducked() = m_bDucked;
		pPlayer->m_ducking() = m_bDucking;
		pPlayer->m_in_duck_jump() = m_bInDuckJump;
		pPlayer->m_model_scale() = m_flModelScale;
	}
};

static CPlayerDataBackup PlayerDataBackup = {};

void CMovementSimulation::SetupMoveData(C_BasePlayer* pPlayer, CMoveData* pMoveData)
{
	IHandleEntity* pEntity = reinterpret_cast<IHandleEntity*>(pPlayer);

	if (!pPlayer || !pMoveData || !pEntity)
		return;

	bFirstRunTick = true;

	pMoveData->m_bFirstRunOfFunctions = false;
	pMoveData->m_bGameCodeMovedPlayer = false;
	pMoveData->m_nPlayerHandle = pEntity->GetRefEHandle();
	pMoveData->m_vecVelocity = pPlayer->m_velocity();
	pMoveData->m_vecAbsOrigin = pPlayer->m_origin();

	//need a better way to determine angles probably
	pMoveData->m_vecViewAngles = { 0.0f, Math::VelocityToAngles(pMoveData->m_vecVelocity).y, 0.0f };

	Vector vForward = {}, vRight = {};
	AngleVectors(pMoveData->m_vecViewAngles, &vForward, &vRight, nullptr);

	pMoveData->m_flForwardMove = (pMoveData->m_vecVelocity.y - vRight.y / vRight.x * pMoveData->m_vecVelocity.x) / (vForward.y - vRight.y / vRight.x * vForward.x);
	pMoveData->m_flSideMove = (pMoveData->m_vecVelocity.x - vForward.x * pMoveData->m_flForwardMove) / vRight.x;
}
#include "CreateMove.h"
bool CMovementSimulation::Initialize(C_BasePlayer* pPlayer)
{
	if (!i::gamemovement || !pPlayer || pPlayer->deadflag())
		return false;

	//set player
	m_pPlayer = pPlayer;

	//set current command
	i::moveHelper->SetHost(pPlayer); // you need to do this, otherwise you will crash.
	pPlayer->SetCurrentCmd(&DummyCmd);

	//store player's data
	PlayerDataBackup.Store(m_pPlayer);

	//store vars
	m_bOldInPrediction = i::prediction->m_bInPrediction;
	m_bOldFirstTimePredicted = i::prediction->m_bFirstTimePredicted;
	m_flOldFrametime = i::globals->frametime;

	//the hacks that make it work
	{
		if (pPlayer->m_flags() & FL_DUCKING)
		{
			pPlayer->m_flags() &= ~FL_DUCKING; //breaks origin's z if FL_DUCKING is not removed
			pPlayer->m_ducked() = true; //(mins/maxs will be fine when ducking as long as m_bDucked is true)
			pPlayer->m_duck_time() = 0.0f;
			pPlayer->m_duck_jump_time() = 0.0f;
			pPlayer->m_ducking() = false;
			pPlayer->m_in_duck_jump() = false;
		}

		if (pPlayer != gLocalPlayer)
			pPlayer->m_ground_entity() = 0; //without this nonlocal players get snapped to the floor

		pPlayer->m_model_scale() -= 0.03125f; //fixes issues with corners

		if (pPlayer->m_flags() & FL_ONGROUND)
			pPlayer->m_origin().z += 0.03125f; //to prevent getting stuck in the ground

		//for some reason if xy vel is zero it doesn't predict
		if (fabsf(pPlayer->m_velocity().x) < 0.01f)
			pPlayer->m_velocity().x = 0.015f;

		if (fabsf(pPlayer->m_velocity().y) < 0.01f)
			pPlayer->m_velocity().y = 0.015f;
	}

	//setup move data
	SetupMoveData(pPlayer, &m_MoveData);
	i::moveHelper->SetHost(nullptr);
	return true;
}
#include "Vars.h"
#include "EntityCache.h"
void CMovementSimulation::FillVelocities()
{
	size_t limit = Vars::ProjectileAim::StrafeChance;
	if (!StrafePredLocal)
	{
		for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES)) {
			if (!pEntity)
				continue;
			if (pEntity->GetTeamNum() == gLocalPlayer->GetTeamNum())
				continue;
			const int iEntIndex = pEntity->GetIndex();

			if (!pEntity->IsAlive() || pEntity->IsDormant()) {
				continue;
			}

			auto& velocities = m_Velocities[iEntIndex];
			velocities.insert(velocities.begin(), m_MoveData.m_vecVelocity);

			if (velocities.size() >= limit) {
				velocities.pop_back();
			}
		}
	}
	else {
		auto& velocities = m_Velocities[gLocalPlayer->GetIndex()];
		velocities.insert(velocities.begin(), m_MoveData.m_vecVelocity);

		if (velocities.size() >= limit) {
			velocities.pop_back();
		}
	}
}
#include "Math.h"

bool CMovementSimulation::BetaStrafePred()
{
	static float flAverageYaw = 0.f;
	static float flInitialYaw = 0.f;

	if (bFirstRunTick)
	{
		bFirstRunTick = false;


		const int iEntIndex = m_pPlayer->GetIndex();
		const auto& mVelocityRecord = m_Velocities[iEntIndex];

		if (static_cast<int>(mVelocityRecord.size()) < 1)
		{
			return false;
		}

		flInitialYaw = Math::VelocityToAngles(m_MoveData.m_vecVelocity).y;
		float flCompareYaw = flInitialYaw;


		for (auto& v : mVelocityRecord)
		{
			const float flRecordYaw = Math::VelocityToAngles(v).y;

			const float flDelta = RAD2DEG(Math::angleDiffRad(DEG2RAD(flCompareYaw), DEG2RAD(flRecordYaw)));
			flAverageYaw += flDelta;

			flCompareYaw = flRecordYaw;
		}

		if (!mVelocityRecord.empty()) {
			flAverageYaw /= mVelocityRecord.size();
		}

		if (!m_pPlayer->IsSolid()) {
			// Handle in-air strafe logic (replace with appropriate movement adjustment)
			m_MoveData.m_flSideMove = flAverageYaw < 0.0f ? 450.0f : -450.0f;
			m_MoveData.m_flForwardMove = 0.0f;
		}
	}

	if (flAverageYaw == 0.f) { return false; }	//	fix

	flInitialYaw += flAverageYaw;
	m_MoveData.m_vecViewAngles.y = flInitialYaw;

	return true;
}

void CMovementSimulation::Restore()
{
	if (!m_pPlayer)
		return;
	i::moveHelper->SetHost(nullptr);
	m_pPlayer->SetCurrentCmd(nullptr);

	PlayerDataBackup.Restore(m_pPlayer);

	i::prediction->m_bInPrediction = m_bOldInPrediction;
	i::prediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	i::globals->frametime = m_flOldFrametime;

	m_pPlayer = nullptr;

	memset(&m_MoveData, 0, sizeof(CMoveData));
	memset(&PlayerDataBackup, 0, sizeof(CPlayerDataBackup));
}
static bool DoOnce = false;
void CMovementSimulation::RunTick()
{
	if (!i::gamemovement || !m_pPlayer)
		return;
	//make sure frametime and prediction vars are right
	i::moveHelper->SetHost(m_pPlayer);
	i::prediction->m_bInPrediction = true;
	i::prediction->m_bFirstTimePredicted = false;
	i::globals->frametime = i::prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	if (!BetaStrafePred())
	{
		m_MoveData.m_vecViewAngles = { 0.0f, Math::VelocityToAngles(m_MoveData.m_vecVelocity).y, 0.0f };
	}
	//gInts.GameMovement->ProcessMovement(m_pPlayer, &m_MoveData);
	//CTFGameMovement::ProcessMovement
	// GETPATT(m_dwProcessMovement, "client.dll", "55 8B EC 56 57 8B 7D 08 8B F1 85 FF 0F 84 ? ? ? ? 83");
	// move this to sigs
	static DWORD Signature = Memory::find_signature<DWORD>("client.dll", "55 8B EC 56 57 8B 7D 08 8B F1 85 FF 0F 84 ? ? ? ? 83");
	reinterpret_cast<void(__thiscall*)(void*, C_BasePlayer*, CMoveData*)>(Signature)(i::gamemovement, m_pPlayer, &m_MoveData);

	i::moveHelper->SetHost(nullptr);
	//return this->GetOrigin();
}
