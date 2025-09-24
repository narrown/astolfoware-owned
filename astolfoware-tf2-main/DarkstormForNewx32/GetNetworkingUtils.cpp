#include "SDK.h"
#include "GetNetworkingUtils.h"
int __cdecl Hookedl0l(int* shit)
{
	oGetNetworkingUtils(shit);

	gInts.NetworkingUtils = reinterpret_cast<ISteamNetworkingUtils*>(shit);

	return oGetNetworkingUtils(shit);
}