#pragma once
#include "SDK.h"

/*
class CEntry
{
public:
	void Initalize();
	void Uninitalize(LPVOID lpArguments);
};
inline CEntry* Entry;
*/

namespace entry {
	void init();
	void uninit(LPVOID lpArguments);
}