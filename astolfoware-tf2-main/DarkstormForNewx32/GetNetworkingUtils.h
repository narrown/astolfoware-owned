#pragma once
#include "SDK.h"

typedef int(__cdecl* oGetNetworkingUtils_t)(int*);
inline oGetNetworkingUtils_t oGetNetworkingUtils;
int __cdecl Hookedl0l(int* shit);