#pragma once
#include "SDK.h"

class CNetworking {
public:
	void CL_SendMove();
	void CL_Move(float AccumulatedExtraSamples, bool FinalTick);
	int GetLatestCommandNumber();
public:
	bool bSendPacket = false;
};
inline CNetworking gNetworking;