#pragma once
#include "dt_common2.h"
#include "dt_recv2.h"
#include <Windows.h>
class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};