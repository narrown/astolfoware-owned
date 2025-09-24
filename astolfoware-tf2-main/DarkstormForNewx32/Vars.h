#pragma once
#include "DrawManager.h"
struct Rect_t
{
	int x, y;
	int width, height;
};

namespace Vars
{
	namespace PriorityOptions
	{
		inline bool IgnoreFriends = true;
		inline bool IgnoreCloaked = true;
	}
	namespace Hitscan
	{
		inline bool bEnable = true;
		inline bool bAutoShoot = true;
		inline bool bKey = true;
		inline bool bSilentAim = true;
		inline int  iFov = 30;
		inline int  MaxPlayers = 2;
		inline int  iHitbox = 0; // 0 - Auto 1 - Head 2 - Body 3 - Pelvis 
		inline int  Key = 8; // R.
		inline bool MultiPointIfNoHitboxesAreVisible;
	}
	namespace AA
	{
		inline bool AA = false;
		inline int Pitch = 0; // 0 - Disabled, 1 - Up, 2 - Down
		inline int FakePitch = 0;// 0 - Disabled 1 - Fake Up 2 - Fake Down
		inline int Yaw = 0; // 0 - Disabled 1 - Right 2 - Left 3 - Back 4 - Emotion 5 - Random 6 - Fake Sideways Right 7 - Fake Sideways Left 8 - Fake Right 9 - Half Back Left 10 - Half Black Right 11 - Fake Left 12 - Fake Manual
	};
	namespace FL
	{
		inline bool Fakelag = false;
		inline int Amount = 2;
	};
	namespace ProjectileAim
	{
		inline bool Enable = true;
		inline int  AimPosition = 0; // 0 - Head, 1 - Body 2 - Feet
		inline int  AimFOV = 35;
		inline bool AutoShoot = true;
		inline bool Silent = true;
		inline bool HeadHitbox = false;
		inline bool BodyHitbox = false;
		inline bool FeetHitbox = false;
		inline int  Key = 8; // R.
		inline float SimulationTime = 10.0;
		inline int StrafeChance = 70;
		inline int StrafeSmooth = 10;
		inline int MaxPlayers = 2;
	}
	namespace ESP
	{
		inline bool Enable = true;
		inline bool Name = true;
		inline bool Class = true;
		inline bool Box = true;
		inline bool Healthbar = true;
		inline bool Conditions = true;
		inline bool EnemyOnly = true;
		inline bool Chams = true; 
		inline bool Distance = true;
	}
	namespace DT
	{
		inline bool Recharging;
		inline bool Warp;
		inline bool DoubleTap = true;
		inline bool AntiWarp = true;
		inline bool shifting = false;
		inline int ticks = 0;
		inline int Shifted = 0;
		inline int DTBarStyle = 1;
		inline int Key = 10;
		inline int WaitForShift;
		inline int ProcessTicks;
		inline CUserCmd* shift_user_cmd = nullptr; // retarded!!!!!!
	}
	namespace Misc
	{
		inline bool Bhop = true;
		inline bool AutoStrafe = true;
		inline bool ThirdPerson = true;
		inline bool isThirdperson;
		inline bool RegionChanger = false;
		inline int  FOV = 120;
		inline bool AutoScoutJump = true;
		inline int AutoScoutJumpKey = 0;
		inline bool PureBypass = true;
		inline bool AutoBackstab = true;
		inline bool F2PBypass = true;
		inline bool SpectatorList = true;
		inline bool StreamerMode = true;
		/* Misc Two */
		inline bool NoiseMaker = false;
		inline bool SkipWelcome = true;
		inline bool AntiAutobalance = true;
		inline bool ReduceInputDelay = true;
		inline bool AutoDetonate = true;
		inline bool GazaBombing  = true;
		inline int GazaBombingKey = 0;
		inline bool GazaBombing_OnlyOnAimbot = false;
		inline float AspectRatio = 1.5; // max 2
		//inline int RegionsAllowed = 0;
		inline bool EUEast      = false;	//EU East		- Frankfurt, Stockholm, Warsaw, Vienna
		inline bool EUWest      = false;	//EU West		- Amsterdam, London, Luxembourg, Madrid, Paris
		inline bool Asia        = false;	//Asia			- Manila, Singapore
		inline bool MiddleEast  = false;	//Middle East	- Dubai
		inline bool HongKong    = false;	//Hong Kong		- Hong Kong
		inline bool Japan       = false;	//Japan			- Tokyo (GNRT)
		inline bool India       = false;	//India			- Mumbai, Chennai
		inline bool Australia   = false;	//Australia		- Sydney
		inline bool Africa      = false;	//Africa		- Johannesburg
		inline bool Chile       = false;	//Chile			- Santiago
		inline bool Peru        = false;	//Peru			- Lima
		inline bool Brazil      = false;	//Brazil		- Sao Paulo
		inline bool USEast      = false;	//US East		- Atlanta, Virginia, Chicago
		inline bool USNorthWest = false;	//US North West - Moses Lake, Washington, Seattle
		inline bool USSouthWest = false;	//US South West - Los Angeles, Oklahoma City
		inline VMatrix m_WorldToProjection = {}; // w2s
		
	}
	namespace CritHax
	{
		inline bool Enable = true;
		inline int CritKey = 9; // CTRL
		inline bool SkipRandomCrits = true;
		inline bool finished_last_shot = false;
	}
	namespace Removals
	{
		inline bool RemoveScope = true;
		inline bool RemoveZoom = true;
	}
}