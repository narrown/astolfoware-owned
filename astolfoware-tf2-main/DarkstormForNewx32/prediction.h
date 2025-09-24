#pragma once
#include "Entities.h"
#include "Enums.h"
#include "Vector.h"
#include "SDKClasses.h"

class IPrediction
{
public:
public:
	virtual		 ~IPrediction(void) {};
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	) = 0;

	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void PostEntityPacketReceived(void) = 0;
	virtual void PostNetworkDataReceived(int commands_acknowledged) = 0;
	virtual void OnReceivedUncompressedPacket(void) = 0;
	virtual void GetViewOrigin(Vector& org) = 0;
	virtual void SetViewOrigin(Vector& org) = 0;
	virtual void GetViewAngles(Vector& ang) = 0;
	virtual void SetViewAngles(Vector& ang) = 0;
	virtual void GetLocalViewAngles(Vector& ang) = 0;
	virtual void SetLocalViewAngles(Vector& ang) = 0;
};
//using entity_handle_t = c_base_handle;
#include "CBaseHandle.h"
typedef CBaseHandle EntityHandle_t;
class CMoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;

	EntityHandle_t m_nPlayerHandle; // edict index on server, client entity handle on client

	int    m_nImpulseCommand; // Impulse command issued.
	Vector m_vecViewAngles; // Command view angles (local space)
	Vector m_vecAbsViewAngles; // Command view angles (world space)
	int    m_nButtons; // Attack buttons.
	int    m_nOldButtons; // From host_client->oldbuttons;
	float  m_flForwardMove;
	float  m_flSideMove;
	float  m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector m_vecVelocity; // edict::velocity		// Current movement direction.
	Vector m_vecAngles; // edict::angles
	Vector m_vecOldAngles;

	// Output only
	float  m_outStepHeight; // how much you climbed this move
	Vector m_outWishVel; // This is where you tried
	Vector m_outJumpVel; // This is your jump velocity

					   // Movement constraints	(radius 0 means no constraint)
	Vector m_vecConstraintCenter;
	float  m_flConstraintRadius;
	float  m_flConstraintWidth;
	float  m_flConstraintSpeedFactor;

	inline void SetAbsOrigin(const Vector& vec)
	{
		m_vecAbsOrigin = vec;
	}
	inline const Vector& GetAbsOrigin() const
	{
		return m_vecAbsOrigin;
	}

	// private:
	Vector m_vecAbsOrigin; // edict::origin
};


class CMoveHelper
{
public:
	void SetHost(C_BasePlayer* host)
	{
		typedef void(__thiscall* FN)(PVOID, C_BasePlayer*);
		Memory::CallVirtual<FN>(this, 12)(this, host);
	}
};

class C_Prediction : public IPrediction
{
public:
	virtual		 ~C_Prediction();
	virtual void Init();
	virtual void Shutdown();
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	virtual void OnReceivedUncompressedPacket();
	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);
	virtual void PostEntityPacketReceived();
	virtual void PostNetworkDataReceived(int commands_acknowledged);
	virtual bool InPrediction();
	virtual bool IsFirstTimePredicted();
	virtual int	 GetIncomingPacketNumber();
	virtual void GetViewOrigin(Vector& org);
	virtual void SetViewOrigin(Vector& org);
	virtual void GetViewAngles(Vector& ang);
	virtual void SetViewAngles(Vector& ang);
	virtual void GetLocalViewAngles(Vector& ang);
	virtual void SetLocalViewAngles(Vector& ang);
	virtual void RunCommand(C_BasePlayer* player, CUserCmd* ucmd, CMoveHelper* moveHelper);
	virtual void SetupMove(C_BasePlayer* player, CUserCmd* ucmd, CMoveHelper* pHelper, CMoveData* move);
	virtual void FinishMove(C_BasePlayer* player, CUserCmd* ucmd, CMoveData* move);
	virtual void SetIdealPitch(C_BasePlayer* player, const Vector& origin, const Vector& angles, const Vector& viewheight);
	virtual void _Update
	(
		bool received_new_world_update,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	//CHandle< C_BaseEntity > m_hLastGround;
	/*
	uint32_t m_hLastGround;
	bool	 m_bInPrediction;
	bool	 m_bFirstTimePredicted;
	bool	 m_bOldCLPredictValue;
	bool	 m_bEnginePaused;
	int		 m_nPreviousStartFrame;
	int		 m_nCommandsPredicted;
	int		 m_nServerCommandsAcknowledged;
	int		 m_bPreviousAckHadErrors;
	int		 m_nIncomingPacketNumber;
	float	 m_flIdealPitch;
	*/
	CHandle<CBaseEntity> m_hLastGround;
	bool m_bInPrediction;
	bool m_bFirstTimePredicted;
	bool m_bOldCLPredictValue;
	bool m_bEnginePaused;
	int m_nPreviousStartFrame;
	int m_nCommandsPredicted;
	int m_nServerCommandsAcknowledged;
	int m_bPreviousAckHadErrors;
	int m_nIncomingPacketNumber;
	float m_flIdealPitch;
};

class IGameMovement
{
public:
	virtual ~IGameMovement(void) {}
};

class C_GameMovement : public IGameMovement
{
public:
	virtual			~C_GameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(C_BasePlayer* pPlayer, CMoveData* pMove) = 0;
	virtual void	StartTrackPredictionErrors(C_BasePlayer* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(C_BasePlayer* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector GetPlayerMins(bool ducked) const = 0;
	virtual Vector GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector GetPlayerViewOffset(bool ducked) const = 0;
};

