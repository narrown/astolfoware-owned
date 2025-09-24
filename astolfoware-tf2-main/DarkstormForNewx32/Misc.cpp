#include "Misc.h"
#include "Vars.h"
#include "Math.h"
#include "Utils.h"

void NoisemakerSpam(PVOID kv) //Credits gir https://www.unknowncheats.me/forum/team-fortress-2-a/141108-infinite-noisemakers.html
{
	char chCommand[30] = "use_action_slot_item_server";
	typedef int(__cdecl* HashFunc_t)(const char*, bool);
	static DWORD dwHashFunctionLocation = gSignature.GetClientSignature("FF 15 ? ? ? ? 83 C4 08 89 06 8B C6");
	static HashFunc_t s_pfGetSymbolForString = (HashFunc_t) * *(PDWORD*)(dwHashFunctionLocation + 0x2);
	*(PDWORD)((DWORD)kv + 0x4) = 0;
	*(PDWORD)((DWORD)kv + 0x8) = 0;
	*(PDWORD)((DWORD)kv + 0xC) = 0;
	*(PDWORD)((DWORD)kv + 0x10) = /*0x10000*/0xDEADBEEF;
	*(PDWORD)((DWORD)kv + 0x14) = 0;
	*(PDWORD)((DWORD)kv + 0x18) = 0; //Extra one the game isn't doing, but if you don't zero this out, the game crashes.
	*(PDWORD)((DWORD)kv + 0x1C) = 0;
	*(PDWORD)((DWORD)kv + 0) = s_pfGetSymbolForString(chCommand, 1);
}

void CMisc::Initalize(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	AutoStrafe(pLocal, pCmd);
	BunnyHop(pLocal, pCmd);
	if (pLocal->GetActiveWeapon())
		auto_scout_jump(pLocal, pLocal->GetActiveWeapon(), pCmd);
	//pLocal->setfov(Vars::Misc::FOV);
	//DoThirdPerson(pLocal, pCmd);
	//DoTauntSpin(pCmd);
	//AutoRocketJump(pCmd);
	if (Vars::Misc::NoiseMaker)
	{
		PVOID kv = Utils::InitKeyValue();
		if (kv != NULL)
		{
			NoisemakerSpam(kv);
			i::engine->ServerCmdKeyValues(kv);
		}
	}
}
inline void fix_movement(CUserCmd* pCmd, const Vector& vecTargetAngle)
{
	Vector vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
	Vector vecMoveAng;

	VectorAngles(vecMove, vecMoveAng);

	float fSpeed = Math::FastSqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
	float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

	pCmd->forwardmove = (cos(fYaw) * fSpeed);
	pCmd->sidemove = (sin(fYaw) * fSpeed);
}
void CMisc::auto_scout_jump(CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd)
{
	if (!Vars::Misc::AutoScoutJump) { return; }

	if (local->deadflag()
		|| local->IsSwimming())
		//|| local->IsAGhost()
		//|| local->IsInBumperKart())
	{
		return;
	}

	if (local->GetMoveType() == MOVETYPE_NOCLIP
		|| local->GetMoveType() == MOVETYPE_LADDER
		|| local->GetMoveType() == MOVETYPE_OBSERVER)
	{
		return;
	}

	static bool shot = false;
	if (Utils::IsKeyPressed(Vars::Misc::AutoScoutJumpKey) && Vars::Misc::AutoScoutJumpKey != 0)
	{
		if (local && !local->deadflag() && !local->IsDormant() && local->GetClassNum() == TF2_Scout && local->IsOnGround())
		{
			if (weapon->GetItemDefinitionIndex() == Scout_m_ForceANature || weapon->GetItemDefinitionIndex() == Scout_m_FestiveForceANature)
			{
				Vector Angles;
				VectorAngles(gLocalPlayer->GetVelocity(), Angles);
				Vector vAngle = { 37.f, Angles.y, 0.0f };
				//Vector vAngle = { 37.f, cmd->viewangles.y, 0.0f };
				//global_info.scout_jumping = true;
				if (!shot)
				{
					cmd->buttons |= IN_JUMP;
					cmd->buttons |= IN_ATTACK;
					shot = true;
				}
				shot = false;

				cmd->viewangles = vAngle;
				fix_movement(cmd, vAngle);
			}
		}
	}
}


void CMisc::BunnyHop(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	if (pLocal)
	{
		if (!Vars::Misc::Bhop
			|| !pLocal->IsAlive()
			|| pLocal->IsSwimming())
			//|| pLocal->IsInBumperKart()
			//|| pLocal->IsAGhost())
			return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !pLocal->IsOnGround())
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
			bJumpState = true;
	}
}
#include "CreateMove.h"
void CMisc::DoThirdPerson()
{
	if (!i::engine->IsInGame())
		return;
	if (!gLocalPlayer)
		return; // IM TIRED OF YOUR SHIT NIGGER. KILL YOURSELF
	if (const auto& pLocal = gLocalPlayer)
	{
		if (!pLocal)
			return;
		if (!pLocal->IsAlive() || !pLocal->GetTeamNum())
			return;

		if (GetAsyncKeyState('B'))
		{
			if (!i::enginevgui->IsGameUIVisible())
			{
				static float flPressedTime = i::engine->Time();
				float flElapsed = i::engine->Time() - flPressedTime;

				if ((GetAsyncKeyState('B') & 0x8000) && flElapsed > 0.2f) {
					Vars::Misc::isThirdperson = !Vars::Misc::isThirdperson;
					flPressedTime = i::engine->Time();
				}
			}
		}

		bool bIsInThirdPerson = i::input->cam_is_third_person();

		if (!Vars::Misc::isThirdperson || ((!Vars::Removals::RemoveScope || !Vars::Removals::RemoveZoom) && pLocal->IsScoped()))
		{
			if (bIsInThirdPerson)
				pLocal->ForceTauntCam(0);

			return;
		}

		if (!bIsInThirdPerson)
			pLocal->ForceTauntCam(1);

		if (bIsInThirdPerson)
		{
			i::prediction->SetLocalViewAngles(qLASTTICK);
		}
	}
}
void CMisc::AutoStrafe(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	if (!Vars::Misc::AutoStrafe)
		return;
	//ConVar* noclip = I::Cvars->FindVar("noclip");
	if (pLocal) {
		static bool was_jumping = false;
		bool is_jumping = pCmd->buttons & IN_JUMP;

		if ((!is_jumping || was_jumping) && !pLocal->IsOnGround())
		{
			Vector Velocity = pLocal->GetVelocity();
			//pLocal->EstimateAbsVelocity(Velocity);
			const float speed = Velocity.Length2D();
			auto vel = Velocity;

			if (pLocal->GetMoveType() == MOVETYPE_NOCLIP
				|| pLocal->GetMoveType() == MOVETYPE_LADDER
				||	pLocal->GetMoveType() == MOVETYPE_OBSERVER)
			{
				return;
			}
			if (pLocal->as<C_BasePlayer>()->m_water_level() > 1)
				return;
			//if (pLocal->GetFlags() & FL_SWIM) // idk if FL_SWIM works l0l!
			//	return;
			if (speed < 2.0f)
				return;

			constexpr auto perfectDelta = [](float speed, CBaseEntity* pLocal) noexcept
			{
				static auto speedVar = pLocal->as<C_BasePlayer>()->m_flMaxspeed();
				static auto airVar = i::cvars->FindVar("sv_airaccelerate");

				static auto wishSpeed = 30.0f;

				const auto term = wishSpeed / airVar->GetFloat() / speedVar * 100.f / speed;

				if (term < 1.0f && term > -1.0f)
					return acosf(term);
				return 0.0f;
			};

			const float pDelta = perfectDelta(speed, pLocal);
			if (pDelta)
			{
				const float yaw = DEG2RAD(pCmd->viewangles.y);
				const float velDir = atan2f(vel.y, vel.x) - yaw;
				const float wishAng = atan2f(-pCmd->sidemove, pCmd->forwardmove);
				const float delta = Math::angleDiffRad(velDir, wishAng);

				float moveDir = delta < 0.0f ? velDir + pDelta : velDir - pDelta;
				 
				pCmd->forwardmove = cosf(moveDir) * 450.f;
				pCmd->sidemove = -sinf(moveDir) * 450.f;
			}


		}
		was_jumping = is_jumping;
	}
}

void CMisc::DoAntiWarp(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<CBaseEntity>();
	if (!pLocal || !pLocal->IsAlive()
		|| !Vars::DT::AntiWarp || !Vars::DT::shifting)
		return;
	Vector Velocity = pLocal->GetVelocity(); 
	float Speed = Velocity.Length2D(); // ZERO VELOCITY ANTI-WARP HELL YEAAAHHH BABY
	INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(i::engine->GetNetChannelInfo());
	if (!pNetChannel || !i::clientstate ||
		Speed < 23.5f)
		return;

	Vector Angles, Forward;
	VectorAngles(Velocity, Angles);
	Vector Viewangles;
	i::engine->GetViewAngles(Viewangles);
	Angles.y = Viewangles.y - Angles.y;
	AngleVectors(Angles, &Forward);
	Forward *= Speed;

	float Scale = (pLocal->as<C_BasePlayer>()->m_flMaxspeed() / Speed) +
		pNetChannel->GetLatency(0);
	
	pCmd->forwardmove = -Forward.x * Scale;
	pCmd->sidemove = -Forward.y * Scale;
}